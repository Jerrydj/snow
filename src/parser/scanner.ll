%{
#include <string>
#include "Scanner.h"
#include "ASTNode.h"

typedef snow::Parser::token token;
typedef snow::Parser::token_type token_type;

#define yyterminate() return token::END_FILE
#define YY_NO_UNISTD_H
%}

%option c++
%option prefix="Snow"
%option batch
%option yywrap nounput 
%option stack

%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% 

%{
    yylloc->step();
%}

[0-9]+                          { yylval->node = new ast::Literal(std::string(yytext), ast::Literal::INTEGER_DEC_TYPE); return token::INTEGER; }
0b[01]+                         { yylval->node = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_BIN_TYPE); return token::INTEGER; }
0x[0-9a-fA-F]+                  { yylval->node = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_HEX_TYPE); return token::INTEGER; }
[0-9]+\.[0-9]+                  { yylval->node = new ast::Literal(std::string(yytext), ast::Literal::FLOAT_TYPE); return token::FLOAT; }
\'(.*)\'                        { yylval->node = new ast::Literal(std::string(yytext), ast::Literal::STRING_TYPE); return token::STRING; }
\"(.*)\"                        { yylval->node = new ast::Literal(std::string(yytext), ast::Literal::STRING_TYPE); return token::STRING; } //'
if                              { return token::IF; }
unless			        	    { return token::UNLESS; }
elsif                           { return token::ELSIF; }
else                            { return token::ELSE; }
do                              { return token::DO; }
while                           { return token::WHILE; }
end                             { return token::END; }
break                           { return token::BREAK; }
continue                        { return token::CONTINUE; }
try                             { return token::TRY; }
catch                           { return token::CATCH; }
throw                           { return token::THROW; }
finally                         { return token::FINALLY; }
return                          { return token::RETURN; }
true			        	    { return token::TRUE; }
false			        	    { return token::FALSE; }
nil						        { return token::NIL; }
and|\&\&                        { return token::LOG_AND; }
or|\|\|                         { return token::LOG_OR; }
not|\!                          { return token::LOG_NOT; }
[_#@a-zA-Z][_#@a-zA-Z0-9]*      { return token::IDENTIFIER; }
≥|>=                            { return token::GTE; }
≤|<=                            { return token::LTE; }
\*\*                            { return token::POW; }
\<\<                            { return token::LSHFT; }
\>\>                            { return token::RSHFT; }
;                               { return token::EOL; }
\n                              { yylloc->lines(yyleng); yylloc->step(); return token::EOL; }
\/\/.+                          { /* Naïve but otherwise good solution - eat short comments. */ }
[ \t\r]                         { yylloc->step(); /* Eat whitespaces */ }
.                               { return static_cast<token_type>(*yytext); }

%%

namespace snow {

    Scanner::Scanner(std::istream* in, std::ostream* out) : SnowFlexLexer(in, out) {
    }

    Scanner::~Scanner() {
    }

}

#ifdef yylex
#undef yylex
#endif

int SnowFlexLexer::yylex() {
    std::cerr << "Warning: In SnowFlexLexer::yylex()" << std::endl;
    return 0;
}

int SnowFlexLexer::yywrap() {
    return 1;
}