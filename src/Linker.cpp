#include "Linker.h"
#include "CompiledCode.h"
#include <iostream>

using namespace std;

namespace snot {
	void Linker::register_symbols(const CompiledCode& code, SymbolTable& table) {
		for (SymbolTable::const_iterator iter = code.symbol_table().begin(); iter != code.symbol_table().end(); ++iter) {
			Symbol ext_symbol = iter->second.to_external(code.code());
			define_symbol(table, iter->first, ext_symbol);
		}
	}
	
	void Linker::link(CompiledCode& code, const SymbolTable& table) {
		vector<Linker::Info>& symrefs = code.symbol_references();
		unsigned char* data = code.code();
		
		for (vector<Linker::Info>::iterator iter = symrefs.begin();;) {
			SymbolTable::const_iterator st_iter = table.find(iter->symbol);
			if (st_iter != table.end()) {
				Symbol symbol = st_iter->second;
				void* address = symbol.address();
				if (iter->relative) {
					void* rel = (void*)(((long long)address - ((long long)data + iter->offset)) + (long long)iter->relative_offset);
					address = rel;
				}
				
				unsigned char* sym_data = reinterpret_cast<unsigned char*>(&address);
				memcpy(&data[iter->offset], sym_data, iter->ref_size);
			} else {
				cerr << "LINKING ERROR: Unresolved symbol: `" << iter->symbol << "'!" << endl;
			}
			
			iter = symrefs.erase(iter);
			if (iter == symrefs.end())
				break;
		}
	}
}