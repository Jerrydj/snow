#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "Basic.h"
#include "Codegen.h"
#include <string>
#include <list>
#include "TempAllocator.h"

namespace snow {
namespace ast {
	struct Node : public TempAllocator<Node> {
		virtual ~Node() {}
		
		template <class T>
		T* as() { return dynamic_cast<T*>(this); }
		template <class T>
		const T* as() const { return dynamic_cast<const T*>(this); }
		template <class T>
		bool is_a() const { return as<T>() != NULL; }
		
		virtual void compile(Codegen& codegen) { TRAP("ast::Node::compile called -- maybe you forgot to override in your Node class?"); }
	};
	
	struct Literal : Node {
		enum Type {
			STRING_TYPE,
			INTEGER_TYPE,
			INTEGER_DEC_TYPE = INTEGER_TYPE,
			INTEGER_HEX_TYPE,
			INTEGER_BIN_TYPE,
			FLOAT_TYPE,
		};
		std::string string;
		Type type;
		
		Literal(const std::string& str, Type type) : string(str), type(type) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Identifier : Node {
		std::string name;
		bool quiet;
		
		Identifier(const std::string& name) : name(name), quiet(false) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Sequence : Node {
		std::list<RefPtr<Node>> nodes;
		
		Sequence() {}
		template <typename... T>
		Sequence(const RefPtr<Node>& first, const T&... args) {
			add(first, args...);
		}
		void add() { }
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) {
			nodes.push_back(node);
			add(args...);
		}
		
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct FunctionDefinition : Node {
		std::list<RefPtr<Identifier>> arguments;
		RefPtr<Sequence> sequence;
		
		FunctionDefinition() : sequence(new Sequence) {}
		template <typename... T>
		FunctionDefinition(const RefPtr<Node>& first, const T&... args) : sequence(new Sequence) { add(first, args...); }
		void add() {}
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) { sequence->add(node, args...); }
        void set_arguments(const std::list<RefPtr<Identifier>>* args) { arguments = (*args); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Return : Node {
		RefPtr<Node> expression;
		
		Return() {}
		Return(const RefPtr<Node>& expr) : expression(expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Assignment : Node {
		RefPtr<Identifier> identifier;
		RefPtr<Node> expression;
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : identifier(ident), expression(expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Condition : Node {
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
	};
	
	struct IfCondition : public Condition {
		RefPtr<Node> if_true;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true) : Condition(expr), if_true(if_true) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct IfElseCondition : public IfCondition {
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false) : IfCondition(expr, if_true), if_false(if_false) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Call : Node {
		RefPtr<Node> object;
		RefPtr<Sequence> arguments;
		Call(RefPtr<Node> obj, RefPtr<Sequence> args = new Sequence) : object(obj), arguments(args) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Send : Node {
		RefPtr<Node> self;
		RefPtr<Node> message;
		Send(RefPtr<Node> self, RefPtr<Node> message) : self(self), message(message) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct MethodCall : Node {
		RefPtr<Node> self;
		RefPtr<Identifier> message;
		RefPtr<Sequence> arguments;
		MethodCall(RefPtr<Node> obj, RefPtr<Identifier> message, RefPtr<Sequence> args = new Sequence) : self(obj), message(message), arguments(args) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Loop : Node {
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */
