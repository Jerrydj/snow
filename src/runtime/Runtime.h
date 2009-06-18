#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "base/Basic.h"
#include "runtime/Value.h"
#include "base/ThreadLocal.h"

namespace snow {
	struct StackFrame;

	VALUE call(VALUE self, VALUE function_or_object, uintx num_args, ...);
	VALUE call_method(VALUE self, const char* name, uintx num_args, ...);
	VALUE get(VALUE self, VALUE sym_member);
	VALUE set(VALUE self, VALUE sym_member, VALUE val);
	bool equals(VALUE a, VALUE b);
	uintx get_object_id(VALUE val);
	
	void set_parent_scope(VALUE func, VALUE scope);
	void enter_scope(Scope* scope, StackFrame* frame);
	void leave_scope();
	StackFrame* get_current_stack_frame();
	ThreadLocal<StackFrame*>& get_current_stack_frames();
	void set_current_stack_frame(StackFrame* frame);
	void update_stack_frame(StackFrame* frame, Scope* scope);
	VALUE get_local(StackFrame* frame, VALUE symbol, bool quiet);
	VALUE set_local(StackFrame* frame, VALUE symbol, VALUE val);
	
	const char* value_to_string(VALUE obj);
	
	/*
		Return Object pointer if the value is an object, otherwise return the
		appropriate prototype.
	*/
	IObject* object_for(VALUE value);

	#ifdef DEBUG
	void print_stack_trace();
	#endif
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
