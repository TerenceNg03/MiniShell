%skeleton "lalr1.cc"

%{
#include <stdio.h>
#include <string>
#include "y.tab.hh"
#include "scanner.hh"
#define yylex driver.scanner_->yylex
%}

%code requires{
    #include "minishell.hpp"
    #include "driver.hh"
    #include "location.hh"
    #include "position.hh"
    #include <tuple>
    #include <filesystem>
    namespace fs = std::filesystem;
    #include <memory>
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
%parse-param {Driver &driver}{minishell& shell}
%lex-param {Driver &driver}{minishell& shell}
%define parse.error verbose
%language "c++"
%define api.value.type variant

%token BG CD CLR DIR _ECHO EXEC EXIT FG MAN JOBS SET SHIFT TEST TIME UNMASK UNSET NEWLINE BACK PIPE UNKNOWN
%token <int> RD_I_AP RD_I RD_O
%token END
%token <std::string> NAME

%type CMDS

%type<command*> BUILT_IN
%type<std::vector<std::tuple<int,int,std::string>>> REDIRECTION
%type<std::vector<std::string>> ARGUMENTS
%type<std::string> BIN
%type<bool> BACKGROUND;
%type<std::pair<int,int>> RD_OP;

%%

CMDS :  /*empty*/{std::cout<<"\nMyshell By Adam Wu\n\nmyshell "<<fs::current_path().filename().string()<<" $ ";}|CMDS CMD NEWLINE {std::cout<<"myshell "<<fs::current_path().filename().string()<<" $ ";};

CMD : BUILT_IN ARGUMENTS REDIRECTION BACKGROUND
{
    $1->set_args($2);
    $1->execute();
}

| BIN ARGUMENTS REDIRECTION BACKGROUND | ;

/* built-in functions */
BUILT_IN :

BG {$$=new command(shell);}
| CD {$$=new cd(shell);}
| CLR {$$=new command(shell);}
| DIR {$$=new command(shell);}
| _ECHO {$$=new command(shell);}
| EXEC {$$=new command(shell);}
| EXIT {YYACCEPT;}
| FG {$$=new command(shell);}
| MAN {$$=new command(shell);}
| JOBS {$$=new command(shell);}
| SET {$$=new command(shell);}
| SHIFT {$$=new command(shell);}
| TEST {$$=new command(shell);}
| TIME {$$=new command(shell);}
| UNMASK {$$=new command(shell);}
| UNSET {$$=new command(shell);}


;

BIN : NAME
{
    $$ = $1;
    std::cout<<$1<<std::endl;
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
