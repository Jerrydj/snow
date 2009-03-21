#ifndef SNOW_DRIVER_H
#define SNOW_DRIVER_H

#include <string>
#include <iostream>
#include "Scanner.h"
#include "ASTNode.h"
#include "RefPtr.h"


namespace snow {

    class Driver
    {
    public:
        inline Driver() {};

        std::string streamname;
        RefPtr<ast::FunctionDefinition> scope;
        Scanner* lexer;

        VALUE parse_stream(std::istream& in, const std::string& sname = "stream input");
        VALUE parse_string(const std::string& input, const std::string& sname = "string stream");
        VALUE parse_file(const std::string& filename);
        VALUE execute();
        inline void error(const location& l, const std::string& m) { std::cerr << l << ": " << m << std::endl; }
        inline void error(const std::string& m) { std::cerr << m << std::endl; }
    };

}
#endif
