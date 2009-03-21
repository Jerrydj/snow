#include <fstream>
#include <sstream>

#include "Driver.h"
#include "Scanner.h"
#include "Codegen.h"
#include "CompiledCode.h"
#include "Linker.h"
#include "lib/Runtime.h"

namespace snow {

    static VALUE global_puts(VALUE self, uint64_t num_args, VALUE* args) {
        for (uint64_t i = 0; i < num_args; ++i) {
            printf("%s\n", value_to_string(args[i]));
        }
        return nil();
    }

    bool Driver::parse_stream(std::istream& in, const std::string& sname) {
        streamname = sname;

        Scanner scanner(&in);
        this->lexer = &scanner;

        Parser parser(*this);
        return (parser.parse() == 0);
    }

    bool Driver::parse_file(const std::string &filename) {
        std::ifstream in(filename.c_str());
        if (!in.good()) return false;
        return parse_stream(in, filename);
    }

    bool Driver::parse_string(const std::string &input, const std::string& sname) {
        std::istringstream iss(input);
        return parse_stream(iss, sname);
    }
    
    VALUE Driver::execute(RefPtr<ast::FunctionDefinition> scope) {
        SymbolTable table;
        table["snow_eval_truth"] = (void*)snow::eval_truth;
        table["snow_call"] = (void*)snow::call;
        table["snow_get"] = (void*)snow::get;
        table["snow_set"] = (void*)snow::set;
        table["snow_enter_scope"] = (void*)snow::enter_scope;
        table["snow_leave_scope"] = (void*)snow::leave_scope;
        table["snow_get_local"] = (void*)snow::get_local;
        
        RefPtr<Codegen> codegen = Codegen::create(*scope);
        RefPtr<CompiledCode> cc = codegen->compile();
        
        cc->export_symbols(table);
        cc->link(table);
        cc->make_executable();
        
        Handle<Scope> global_scope = new Scope;
    	auto gp = new Function(global_puts);
    	global_scope->set_local("puts", gp);
        
        Handle<Function> f = new Function(*cc);
        f->set_parent_scope(global_scope);
        return f->call(nil(), 0);
    }

}
