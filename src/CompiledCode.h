#ifndef COMPILEDCODE_H_1G9CD943
#define COMPILEDCODE_H_1G9CD943

#include <vector>
#include <string>
#include "SymbolTable.h"
#include "Linker.h"

namespace snot {
	class CompiledCode {
	private:
		unsigned char* m_Code;
		int m_Size;
		std::vector<Linker::Info> m_SymbolReferences;
		SymbolTable m_SymbolTable;
	public:
		explicit CompiledCode(int size);
		virtual ~CompiledCode();
		
		inline int size() const { return m_Size; }
		inline unsigned char* code() { return m_Code; }
		inline const unsigned char* code() const { return m_Code; }
		inline const SymbolTable& symbol_table() const { return m_SymbolTable; }
		inline std::vector<Linker::Info>& symbol_references() { return m_SymbolReferences; }
		
		void set_symbol(const std::string& name, int offset);
		void set_symbol_reference(const Linker::Info& info);
		
		void make_executable();
	};
}

#endif /* end of include guard: COMPILEDCODE_H_1G9CD943 */
