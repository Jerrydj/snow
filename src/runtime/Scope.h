#ifndef SCOPE_H_KJ3A0580
#define SCOPE_H_KJ3A0580

#include "runtime/ThinObject.h"
#include "runtime/Array.h"
#include "runtime/Function.h"

namespace snow {
	Object* scope_prototype();
	void scope_prototype_init();
	
	class Scope : public ThinObject {
	private:		
		GC_ROOTS;

		VALUE m_Self;
		Function* m_Function;
		LocalMap* m_LocalMap;
		Array* m_Arguments;
		Array* m_Locals;
		Scope* m_CallingScope;
		bool m_GCLock;
		
		bool gc_try_lock() { bool v = !m_GCLock; if (v) { m_GCLock = true; } return v; }
		void gc_unlock() { m_GCLock = false; }
	public:
		explicit Scope(Function* func = NULL);
		void initialize(Function* func = NULL);
		Scope(const Scope& other); 
		void initialize(const Scope& other) {}
		virtual ~Scope() {}

		IObject* copy() const { return gc_new<Scope>(*this); }
		
		VALUE self() const;
		void set_self(VALUE self) { m_Self = self; }
		
		Function* function() const { return m_Function; }
		
		Array* locals() const { return m_Locals; }
		LocalMap* local_map() const { return m_LocalMap; }
		bool has_local(VALUE name);
		VALUE get_local(VALUE name);
		VALUE set_local(VALUE name, VALUE value);
		VALUE get_local_by_string(const char* s) { return get_local(symbol(s)); }
		VALUE set_local_by_string(const char* s, VALUE val) { return set_local(symbol(s), val); }
		
		Array* arguments() const { return m_Arguments; }
		void set_arguments(Array* args) { m_Arguments = args; }
		
		Scope* calling_scope() const { return m_CallingScope; }
		void set_calling_scope(Scope* scope) { m_CallingScope = scope; }
	};
}

#endif /* end of include guard: SCOPE_H_KJ3A0580 */
