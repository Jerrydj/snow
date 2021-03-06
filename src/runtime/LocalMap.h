#ifndef LOCALMAP_H_QGZ58WXV
#define LOCALMAP_H_QGZ58WXV

#include <unordered_map>
#include "base/Basic.h"
#include "gc/IGarbage.h"
#include "runtime/Value.h"

namespace snow {
	/*
		A LocalMap is a mapping from the name of a local variable in a scope
		to the index of the same local variable in that scope's array of
		locals.
		
		This is part of the optimization that allows locals to have constant
		lookup time, because we know the contents of the LocalMap at compile-time.

		The map can be changed at runtime as well, though.
		This happens in global scope when external source files are included.
	*/
	class LocalMap : public IGarbage {
	private:
		std::unordered_map<VALUE, uintx> m_Map;
		uintx m_NumLocals;
		uintx m_NumArguments;

		GC_ROOTS {}
		bool gc_try_lock() { return true; }
		void gc_unlock() { }
	public:
		NO_INIT;

		LocalMap() : m_NumLocals(0), m_NumArguments(0) {}
		LocalMap(const LocalMap& other) : m_Map(other.m_Map), m_NumLocals(other.m_NumLocals) {}
		
		bool has_local(VALUE name) const { return m_Map.find(name) != m_Map.end(); }
		uintx local(VALUE name) const;
		uintx define_local(VALUE name);
		uintx define_argument(VALUE name) { ++m_NumArguments; return define_local(name); }
		
		uintx size() const { return m_NumLocals; }
		uintx num_arguments() const { return m_NumArguments; }
		const std::unordered_map<VALUE, uintx>& map() const { return m_Map; }
	};
}

#endif /* end of include guard: LOCALMAP_H_QGZ58WXV */
