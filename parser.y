%skeleton "lalr1.cc"

%{
#include <stdio.h>
#include <string>
#include "y.tab.hh"
#include "scanner.hh"
#define yylex driver.scanner_->yylex
%}

%code requires{
    #include "myshell.hpp"
    #include "driver.hh"
    #include "location.hh"
    #include "position.hh"
    #include <tuple>
}

%code provides
{
    namespace parse
    {
        // Forward declaration of the Driver class
        class Driver;

        inline void
        yyerror (const char* msg)
        {
            std::cout << msg << std::endl;
        }
    }
}

%locations
%define api.namespace {parse}
%define api.parser.class {Parser}
%parse-param {Driver &driver}{myshell& shell}
%lex-param {Driver &driver}{myshell& shell}
%define parse.error verbose
%language "c++"
%define api.value.type variant

%token BG CD CLR DIR _ECHO EXEC EXIT FG MAN JOBS SET SHIFT TEST TIME UNMASK UNSET NEWLINE BACK PIPE UNKNOWN
%token <int> RD_I_AP RD_I RD_O
%token END
%token <std::string> NAME

%type CMDS

%type<command> COMMAND_NAME
%type<command> BUILT_IN BIN
%type<std::vector<std::tuple<int,int,std::string>>> REDIRECTION
%type<std::vector<std::string>> ARGUMENTS
%type<bool> BACKGROUND;
%type<std::pair<int,int>> RD_OP;

%%
CMDS :  /*empty*/|CMDS CMD NEWLINE;

CMD : COMMAND_NAME ARGUMENTS REDIRECTION BACKGROUND | ;

COMMAND_NAME : BUILT_IN | BIN;

/* built-in functions */
BUILT_IN :

BG {$$=command();}
| CD {$$=command();}
| CLR {$$=command();}
| DIR {$$=command();}
| _ECHO {$$=command();}
| EXEC {$$=command();}
| EXIT {YYACCEPT;}
| FG {$$=command();}
| MAN {$$=command();}
| JOBS {$$=command();}
| SET {$$=command();}
| SHIFT {$$=command();}
| TEST {$$=command();}
| TIME {$$=command();}
| UNMASK {$$=command();}
| UNSET {$$=command();}


;

BIN : NAME{$$=command();std::cout<<$1<<std::endl;
    FILE* fp = fopen("output.txt","w");
    fprintf(fp,"Hello");
    fclose(fp);
};

/* arguments */
ARGUMENTS : {$$ = std::vector<std::string>();} | ARGUMENTS NAME {std::swap($$, $1);$$.push_back($2);};

/* redirection */

REDIRECTION :  /* empty */{$$ = std::vector<std::tuple<int,int,std::string>>();} | REDIRECTION RD_OP NAME

{
    std::swap($$,$1);
    $$.push_back(std::tuple<int,int,std::string>($2.first,$2.second,$3));

};

RD_OP : RD_I {$$ = std::make_pair($1,0);} | RD_O {$$ = std::make_pair($1,1);} | RD_I_AP {$$ = std::make_pair($1,2);};

/* background */

BACKGROUND : {$$ = false;} | BACK {$$ = true;};

%%

namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cout << *driver.location_ << ": " << m << std::flush;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
