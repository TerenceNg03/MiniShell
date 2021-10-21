%{
#include <iostream>
#include "y.tab.hh"
#include "scanner.hh"
#include <filesystem>
namespace fs = std::filesystem;
#define yyterminate() return( parse::Parser::token::END )

#ifndef register
#define register
#endif

#define STEP()                                      \
do {                                              \
driver.location_->step ();                      \
} while (0)

#define COL(Col)                                    \
driver.location_->columns (Col)

#define LINE(Line)                                    \
do{                                                \
driver.location_->lines (Line);                    \
} while (0)

#define YY_USER_ACTION                                \
COL(yyleng);

#define yywrap() 1

%}

%option noinput nounput 8bit nodefault
%option debug
%option noyywrap
%option prefix="parse"

%option c++
%%

bg return parse::Parser::token::BG;
cd return parse::Parser::token::CD;
echo return parse::Parser::token::_ECHO;
exec return parse::Parser::token::EXEC;
exit return parse::Parser::token::EXIT;
fg return parse::Parser::token::FG;
set return parse::Parser::token::SET;
shift return parse::Parser::token::SHIFT;
test return parse::Parser::token::TEST;
time return parse::Parser::token::TIME;
umask return parse::Parser::token::UNMASK;
unset return parse::Parser::token::UNSET;

[1-2]">>" yylval->build(yytext[0]-'0');return parse::Parser::token::RD_O_AP;
[1-2]">" yylval->build(yytext[0]-'0');return parse::Parser::token::RD_O;
[0]"<" yylval->build(yytext[0]-'0');return parse::Parser::token::RD_I;

[\'\"].*[\'\"]  yylval->build(std::string(yytext));return parse::Parser::token::STR;

[a-zA-Z0-9\-\+]{1,} yylval->build(std::string(yytext));return parse::Parser::token::ID;

"$"[a-zA-Z0-9\-]+ yylval->build(std::string(yytext+1));return parse::Parser::token::VAR;

[a-zA-Z0-9\-\./~\+]{1,} yylval->build(std::string(yytext));return parse::Parser::token::_PATH;


"&" return parse::Parser::token::BACK;
"|" return parse::Parser::token::PIPE;
"=" return parse::Parser::token::ASSIGN;

\n return parse::Parser::token::NEWLINE;
<<EOF>> return parse::Parser::token::_EOF;
[ \t]+ /* ignore whitespace */
. yylval->build(std::string(yytext)); return parse::Parser::token::UNKNOWN;

%%

namespace parse
{

    Scanner::Scanner()
    : parseFlexLexer()
    {
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::set_debug(bool b)
    {
        yy_flex_debug = b;
    }
}

#ifdef yylex
# undef yylex
#endif


int parseFlexLexer::yylex()
{
  std::cerr << "call parsepitFlexLexer::yylex()!" << std::endl;
  return 0;
}
