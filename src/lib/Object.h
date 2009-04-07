#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include "ThinObject.h"

#include <stdarg.h>
#include <map>
#include <unordered_map>
#include <string>

namespace snow {
class Object;

Object* object_prototype();

class Object : public ThinObject {
public:
	// TODO: Use a more GC-friendly hashmap
	typedef std::unordered_map<VALUE, ValueHandle> Members;
private:
	Members m_Members; 
public:
	explicit Object(const Handle<Object>& prototype = NULL) : ThinObject(prototype) {}
	Object(const Object& other) : ThinObject(other), m_Members(other.m_Members) {}
	virtual ~Object() {}
	
	VALUE va_call(VALUE self, uint64_t num_args, va_list&);
	const Members& members() const { return m_Members; }
	virtual bool has_member(VALUE member) const;
	virtual VALUE set(VALUE member, VALUE value);
	virtual VALUE get(VALUE member) const;
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
