%{
#include <stdio.h>
#include "y.tab.hh"
#include "scanner.hh"

#define yyterminate() return( parse::Parser::token::END )

%}

%option noinput nounput noyywrap 8bit nodefault
%option debug
%option noyywrap
%option never-interactive
%option yylineno
%option batch
%option prefix="parse"

%option yylineno
%option c++
%%

bg return parse::Parser::token::BG;
cd return parse::Parser::token::CD;
clr return parse::Parser::token::CLR;
dir return parse::Parser::token::DIR;
echo return parse::Parser::token::_ECHO;
exec return parse::Parser::token::EXEC;
exit return parse::Parser::token::EXIT;
fg return parse::Parser::token::FG;
man return parse::Parser::token::MAN;
ps return parse::Parser::token::PS;
set return parse::Parser::token::SET;
shift return parse::Parser::token::SHIFT;
test return parse::Parser::token::TEST;
time return parse::Parser::token::TIME;
umask return parse::Parser::token::UNMASK;
unset return parse::Parser::token::UNSET;

[a-zA-Z0-9\-\.] /*yylval->s=new std::string(yytext);*/return parse::Parser::token::NAME;
"&" return parse::Parser::token::BACK;
"|" return parse::Parser::token::PIPE;
">>" parse::Parser::token::RD_I_AP;
">" parse::Parser::token::RD_I;
"<" parse::Parser::token::RD_O;

\n return parse::Parser::token::NEWLINE;
[ \t]+ /* ignore whitespace */
. return parse::Parser::token::UNKNOWN;

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
