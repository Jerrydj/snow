#include "Float.h"
#include "Numeric.h"
#include "Function.h"
#include "SnowString.h"
#include <sstream>

namespace snow {	
	static VALUE float_to_i(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT(is_float(self));
		int64_t a = (int64_t)floatnum(self);
		return value(a);
	}
	
	static VALUE float_to_f(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT(is_float(self));
		return self;
	}
	
	static VALUE float_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args == 0);
		std::stringstream ss;
		ss << floatnum(self);
		return value(gc_new<String>(ss.str()));
	}
	
	Object* float_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw_s("+", gc_new<Function>(numeric_add));
		proto->set_raw_s("-", gc_new<Function>(numeric_sub));
		proto->set_raw_s("*", gc_new<Function>(numeric_mul));
		proto->set_raw_s("/", gc_new<Function>(numeric_div));
		proto->set_raw_s("%", gc_new<Function>(numeric_mod));
		proto->set_raw_s("**", gc_new<Function>(numeric_power));
		proto->set_raw_s("<", gc_new<Function>(numeric_lt));
		proto->set_raw_s("<=", gc_new<Function>(numeric_lte));
		proto->set_raw_s(">", gc_new<Function>(numeric_gt));
		proto->set_raw_s(">=", gc_new<Function>(numeric_gte));
		
		VALUE to_string = gc_new<Function>(float_to_string);
		proto->set_raw_s("to_string", to_string);
		proto->set_raw_s("inspect", to_string);
		proto->set_raw_s("to_f", gc_new<Function>(float_to_f));
		proto->set_raw_s("to_i", gc_new<Function>(float_to_i));
		return proto;
	}
}
