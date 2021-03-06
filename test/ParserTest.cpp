#include "test.h"
#include "runtime/Kernel.h"
#include "parser/Driver.h"
#include <string>
#include <cmath>

using namespace snow;

TEST_SUITE(Parser);

TEST_CASE(assignments) {
	intx correct = 3;
	Value v = Kernel::eval("h: 3");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(functions) {
	intx correct = 26;
	Value v = Kernel::eval("double: [a] { a*2 }; double(12)+2");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(properties) {
	intx correct = 3;
	Value v = Kernel::eval("@(1,2,3).length");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(elseif) {
	intx correct = 30;
	Value v = Kernel::eval("i: 25; if i < 10; 10; else if i < 20; 20; else if i < 30; 30; else; 40; end");
	TEST_EQ(integer(v.value()), correct);

	correct = 40;
	v = Kernel::eval("i: 35; if i < 10; 10; else if i < 20; 20; else if i < 30; 30; else; 40; end");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(long_conditionals) {
	intx correct = 3;
	Value v = Kernel::eval("i: 3; if i > 2; i; else; i: i * 2; end");
	TEST_EQ(integer(v.value()), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; unless i > 2; i; else; i: i * 2; end");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(short_conditionals) {
	intx correct = 3;
	Value v = Kernel::eval("i: 3; unless i > 2; return i * 2; end; i");
	TEST_EQ(integer(v.value()), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; if i > 2; return i * 2; end; i");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(inline_conditionals) {
	intx correct = 3;
	Value v = Kernel::eval("i: 3; return i * 2 unless i > 2; i");
	TEST_EQ(integer(v.value()), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; return i * 2 if i > 2; i");
	TEST_EQ(integer(v.value()), correct);
	
	v = Kernel::eval("#foo unless true");
	TEST_EQ(v, nil());
	
	v = Kernel::eval("#foo if false");
	TEST_EQ(v, nil());
}

TEST_CASE(addition) {
	intx correct = 19+213;
	Value v = Kernel::eval("19+213");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(subtraction) {
	intx correct = 19-213;
	Value v = Kernel::eval("19-213");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(unary_minus) {
	intx correct = -19;
	Value v = Kernel::eval("-19");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(multiplication) {
	intx correct = 19*213;
	Value v = Kernel::eval("19*213");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(division) {
	intx correct = 22/7;
	Value v = Kernel::eval("22/7");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(modulus) {
	intx correct = 22 % 7;
	Value v = Kernel::eval("22 % 7");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(power) {
	intx correct = pow(9,4);
	Value v = Kernel::eval("9 ** 4");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(Symbols) {
	Value correct1 = Symbol("hej");
	Value v1 = Kernel::eval("#hej");
	
	Value correct2 = Symbol("hej dav");
	Value v2 = Kernel::eval("#\"hej dav\"");
		
	TEST_EQ(v1, correct1);
	TEST_EQ(v2, correct2);
}

TEST_CASE(associativity) {
	intx correct = 5+4*3;
	Value v = Kernel::eval("5+4*3");
	TEST_EQ(integer(v.value()), correct);
}

TEST_CASE(string_interpolation) {
	std::string correct = std::string("hej 7 dav 2");
	Value v = Kernel::eval("\"hej ${3+4} dav ${5-3}\"");
	TEST_EQ(value_to_string(v), correct);
	
	correct = std::string("Hello World!");
	v = Kernel::eval("hello: [who] { \"Hello ${who}!\"}; hello(\"World\")");
	TEST_EQ(value_to_string(v), correct);
	
	correct = std::string("Hello, this is TestObject.to_string()");
	v = Kernel::eval("TestObject: Object.new(); TestObject.to_string: { \"this is TestObject.to_string()\"}; \"Hello, ${TestObject}\"");
	TEST_EQ(value_to_string(v), correct);
	
	correct = std::string("8 * 3 = 24");
	v = Kernel::eval("mul: [a,b] {a * b}; \"8 * 3 = ${mul(8,3)}\"");
	TEST_EQ(value_to_string(v), correct);
}

TEST_CASE(octal_string_escapes) {
	std::string correct = std::string("$");
	Value v = Kernel::eval("\"\\044\""); // (char)044 == '$'
	TEST_EQ(value_to_string(v), correct);
	
	correct = std::string("0");
	v = Kernel::eval("\"\\060\""); // (char)060 == '0'
	TEST_EQ(value_to_string(v), correct);
	
	correct = std::string("\n");
	v = Kernel::eval("\"\\012\""); // (char)012 == '\n'
	TEST_EQ(value_to_string(v), correct);
}