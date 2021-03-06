#ifndef _EXTERNAL_LIBRARY_H_A4LKJ87
#define _EXTERNAL_LIBRARY_H_A4LKJ87

#include "runtime/Object.h"

namespace snow {
	class Function;

	Ptr<Object> external_library_prototype();

	class ExternalLibrary : public Object {
	private:
		void* m_Handle;
	public:
		enum NativeType {
			NATIVE_VOID,
			NATIVE_POINTER,
			NATIVE_INT,
			NATIVE_FLOAT,
			NATIVE_STRING,
			NATIVE_VALUE,
		};

		struct FunctionSignature {
			NativeType return_type;
			uintx num_args;
			NativeType* arg_types; 

			FunctionSignature(uintx num_args) : num_args(num_args) {
				if (num_args > 0)
					arg_types = new NativeType[num_args];
				else
					arg_types = NULL;
			}

			~FunctionSignature() {
				delete[] arg_types;
			}
		};

		explicit ExternalLibrary(void* handle) : Object(external_library_prototype()), m_Handle(handle) {}
		void initialize(void* handle) {}
		~ExternalLibrary();

		Ptr<Function> compile_proxy(const char* symbol_name, const FunctionSignature&);
	};
	
	/*
		Functions for the ExternalLibrary code generation
	*/
	void external_library_function_num_args_mismatch(uintx expected, uintx given);
	void* convert_value_to_native(VALUE, ExternalLibrary::NativeType type);
	VALUE convert_native_to_value(void*, ExternalLibrary::NativeType type);

}

#endif // _EXTERNAL_LIBRARY_H_A4LKJ87
