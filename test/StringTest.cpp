#include <string>
#include <sstream>
#include <iostream>
#include "test.h"
#include "runtime/Runtime.h"
#include "runtime/SnowString.h"

using namespace snow;

TEST_SUITE(String);

static const std::string a("foo");
static const std::string b("bar");

TEST_CASE(concatenation) {
	HandleScope _s;
	std::stringstream ss;
	ss << a << b;
	std::string correct = ss.str();
	
	VALUE self = gc_new<String>(a.c_str());
	VALUE arg = gc_new<String>(b.c_str());
	
	auto res1 = object_cast<String>(snow::call_method(self, "+", 1, arg));
	TEST_EQ(res1->str(), ss.str());
	
	auto res2 = object_cast<String>(snow::call_method(self, "<<", 1, arg));
	TEST_EQ(res2->str(), res1->str());
}

TEST_CASE(reverse) {
	HandleScope _s;
	VALUE self = gc_new<String>(a.c_str());
	std::string correct("oof");
	auto result = object_cast<String>(snow::call_method(self, "reverse", 0));
	TEST_EQ(result->str(), correct);
}
