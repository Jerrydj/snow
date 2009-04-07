#include "Linker.h"
#include "CompiledCode.h"
#include "Basic.h"

using namespace std;

namespace snow {
	void Linker::link(CompiledCode& code, const SymbolTable& table) {
		auto symrefs = code.symbol_references();
		byte* data = code.code();
		
		for (auto iter = symrefs.begin(); iter != symrefs.end();) {
			auto st_iter = table.find(iter->symbol);
			if (st_iter != table.end()) {
				Symbol symbol = st_iter->second;
				void* address = symbol.address();
				if (iter->relative) {
					void* rel = (void*)(((int64_t)address - ((int64_t)data + iter->offset)) + (int64_t)iter->relative_offset);
					address = rel;
				}
				
				byte* sym_data = reinterpret_cast<byte*>(&address);
				memcpy(&data[iter->offset], sym_data, iter->ref_size);
			} else {
				error("(linker) Unresolved symbol: `%s'!", iter->symbol.c_str());
			}
			
			iter = symrefs.erase(iter);
		}
	}

	Linker::Symbol Linker::define_symbol(Linker::SymbolTable& table, const std::string& name, const Symbol& symb) {
		auto existing = table.find(name);
		if (existing != table.end() && existing->second != symb) {
			warn("Symbol `%s' is already defined in table 0x%lx, overwriting...\n", name.c_str(), &table);
		}
		table.insert(SymbolTable::value_type(name, symb));
		return symb;
	}
}
