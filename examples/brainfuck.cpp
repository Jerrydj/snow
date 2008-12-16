#include "x86_64/Assembler.h"
#include "Linker.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include <queue>

using namespace snot;
using namespace snot::x86_64;

template <typename T>
void store_ptr(x86_64::Assembler& m, T* ptr, const Address& addr) {
	m.mov(Immediate(&ptr, sizeof(T*)), rax);
	m.mov(rax, addr);
}

static bool moved_since_last_load = true;

void load_cell_addr_to_rax(x86_64::Assembler& m, bool force = false) {
	if (moved_since_last_load || force) {
		m.mov(Address(rbp, -8), rax);
		m.add(Address(rbp, -16), rax);
		moved_since_last_load = false;
	}
}

void inc(x86_64::Assembler& m) {
	load_cell_addr_to_rax(m);
	m.incb(Address(rax));
}

void dec(x86_64::Assembler& m) {
	load_cell_addr_to_rax(m);
	m.decb(Address(rax));
}

void move_left(x86_64::Assembler& m) {
	m.dec(Address(rbp, -16));
	moved_since_last_load = true;
}

void move_right(x86_64::Assembler& m) {
	m.inc(Address(rbp, -16));
	moved_since_last_load = true;
}

void put_char(x86_64::Assembler& m) {
	load_cell_addr_to_rax(m);
	m.mov(Address(rax), rdi);
	m.call("putchar");
	moved_since_last_load = true;
}

void get_char(x86_64::Assembler& m) {
	load_cell_addr_to_rax(m);
	m.mov(Address(rax), rdi);
	m.call("getchar");
	m.mov(rax, rbx);
	load_cell_addr_to_rax(m, true);
	m.mov(rbx, Address(rax));	// XXX: moves 8 bytes, should be 1
}


static std::queue<Label> jump_forward_labels;
static std::queue<Label> jump_backward_labels;

void jump_forward(x86_64::Assembler& m) {
	Label front;
	Label back;
	
	load_cell_addr_to_rax(m);
	m.bin_xor(rbx, rbx);
	m.cmpb(Address(rax), rbx);
	m.j(CC_EQUAL, back);
	m.bind(front);
	
	jump_forward_labels.push(front);
	jump_backward_labels.push(back);
}

void jump_backward(x86_64::Assembler& m) {
	if (jump_forward_labels.empty() || jump_backward_labels.empty()) {
		fprintf(stderr, "ERROR: Unmatched ]\n");
	} else {
		Label front = jump_forward_labels.front();
		jump_forward_labels.pop();
		Label back = jump_backward_labels.front();
		jump_backward_labels.pop();
		
		load_cell_addr_to_rax(m);
		m.bin_xor(rbx, rbx);
		m.cmpb(Address(rax), rbx);
		m.j(CC_NOT_EQUAL, front);
		m.bind(back);
	}
}

const char* bf = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++>++++++[<.>-]>++++++++++.";
const char* hello = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

int main (int argc, char const *argv[])
{
	x86_64::Assembler m;
	SymbolTable table;
	table["putchar"] = Symbol((void*)putchar);
	table["getchar"] = Symbol((void*)getchar);
	
	unsigned char* buffer = (unsigned char*)malloc(1 << 16);
	memset(buffer, 0, (1 << 16));
	
	m.enter(Immediate(32));
	store_ptr(m, buffer, Address(rbp, -8));
	m.bin_xor(rax, rax);
	m.mov(rax, Address(rbp, -16));
	
	const char* input = hello;
	
	size_t len = strlen(input);
	int i;
	for (i = 0; i < len; ++i) {
		switch (input[i]) {
		case '+':
			inc(m);
			break;
		case '-':
			dec(m);
			break;
		case '>':
			move_right(m);
			break;
		case '<':
			move_left(m);
			break;
		case '.':
			put_char(m);
			break;
		case ',':
			get_char(m);
			break;
		case '[':
			jump_forward(m);
			break;
		case ']':
			jump_backward(m);
			break;
		default:
			break;
		}
	}

	m.bin_xor(rax, rax);
	m.leave();
	m.ret();
	
	CompiledCode code = m.compile();
	Linker::link(code, table);
	
	code.make_executable();
	
	void(*func)() = (void(*)())code.code();
	func();
	
	free(buffer);
	return 0;
}