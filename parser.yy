%skeleton "lalr1.cc"

%{
#include <stdio.h>
#include <string.h>
#include "y.tab.hh"
#include "scanner.hh"
#define yylex driver.scanner_->yylex
%}

%code requires{
    #include "driver.hh"
    #include "location.hh"
    #include "position.hh"
}

%locations
%defines
%debug
%define api.namespace {parse}
%define api.parser.class {Parser}
%parse-param {Driver &driver}
%lex-param {Driver &driver}
%define parse.error verbose
%language "c++"
%union
{
    int i;
    std::string* s;
    double f;
}

%token BG CD CLR DIR _ECHO EXEC EXIT FG MAN PS SET SHIFT TEST TIME UNMASK UNSET NEWLINE BACK PIPE RD_I_AP RD_I RD_O UNKNOWN
%token END
%token <s> NAME

%type CMDS CMD
%type BUILT_IN BIN
%type ARGUMENTS

%%
CMDS :  /*empty*/ |CMDS CMD NEWLINE;

CMD : BUILT_IN | BIN ARGUMENTS;

BUILT_IN : DIR;

BIN : NAME;

ARGUMENTS : | ARGUMENTS NAME;
%%

namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cerr << *driver.location_ << ": " << m << std::endl;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}