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
    #include <unistd.h>
    #include <signal.h>
    #include <sys/wait.h>
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
            std::cout << msg << "\n";
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

%token BG CD _ECHO EXEC EXIT FG SET SHIFT TEST TIME UNMASK UNSET UNKNOWN
%token NEWLINE BACK PIPE ASSIGN
%token <int> RD_O_AP RD_I RD_O
%token _EOF
%token <std::string> VAR STR ID _PATH

%type CMDS
%type<std::string> NAME PATH
%type<command*> BUILT_IN
%type<std::vector<std::tuple<int,int,std::string>>> REDIRECTION
%type<std::vector<std::string>> ARGUMENTS
%type<std::string> BIN
%type<std::pair<int,int>> RD_OP;

%%

CMDS :  /*empty*/{std::cout<<"\nMyshell By Terence Ng\n\nminishell "<<fs::path(shell.env["PWD"]).filename().string()<<" $ ";}|CMDS CMD NEWLINE {std::cout<<"minishell "<<fs::path(shell.env["PWD"]).filename().string()<<" $ ";};


CMD : | BUILT_IN ARGUMENTS REDIRECTION BACK CMD
{
    $1->set_args($2);
    std::ifstream* i;
    std::ofstream* o;
    for(auto rd:$3){
        switch(std::get<1>(rd)){
            case 0:
            i = new std::ifstream(std::get<2>(rd));break;
            case 1:
            o = new std::ofstream(std::get<2>(rd),std::ios_base::trunc);break;
            case 2:
            o = new std::ofstream(std::get<2>(rd),std::ios_base::app);break;
        }
        switch(std::get<0>(rd)){
            case 0:
            $1->set_is((std::istream*)i);break;
            case 1:
            $1->set_os((std::ostream*)o);break;
            case 2:
            $1->set_es((std::ostream*)o);break;
        }

    }

    pid_t pid = fork();
    if(pid==-1){
        std::cerr<<"Error : Unable create new process.\n";
        return -1;
    }else if(pid==0){
        signal(SIGTSTP,SIG_DFL);
        int ret = $1->execute();
        exit(ret);
    }
    if($1)delete $1;
    if(i)delete i;
    if(o)delete o;
}

|BUILT_IN ARGUMENTS REDIRECTION
{
    $1->set_args($2);
    std::ifstream* i;
    std::ofstream* o;
    for(auto rd:$3){
        switch(std::get<1>(rd)){
            case 0:
            i = new std::ifstream(std::get<2>(rd));break;
            case 1:
            o = new std::ofstream(std::get<2>(rd),std::ios_base::trunc);break;
            case 2:
            o = new std::ofstream(std::get<2>(rd),std::ios_base::app);break;
        }
        switch(std::get<0>(rd)){
            case 0:
            $1->set_is((std::istream*)i);break;
            case 1:
            $1->set_os((std::ostream*)o);break;
            case 2:
            $1->set_es((std::ostream*)o);break;
        }

    }

    $1->execute();

    if($1)delete $1;
    if(i)delete i;
    if(o)delete o;
}


| BIN ARGUMENTS REDIRECTION BACK CMD
{
    if($1!=""){
        pid_t pid = fork();

        if(pid==-1){
            std::cerr<<"Error : Unable create new process.\n";
        }else if(pid==0){

            signal(SIGTSTP,SIG_DFL);
            signal(SIGCHLD,SIG_DFL);

            std::vector<const char*> argv;
            argv.push_back($1.c_str());
            for(int i=0; i<$2.size(); i++){
                argv.push_back($2[i].c_str());
            }
            argv.push_back(nullptr);

            int ret = execv($1.c_str(),(char *const *)&argv[0]);
            std::cerr<<"Exec Failed with Error Code  "<<errno<<"\n";
            exit(ret);
        }
    }
}

| BIN ARGUMENTS REDIRECTION
{
    if($1!=""){
        shell.waiting = true;
        pid_t pid = fork();

        if(pid==-1){
            std::cerr<<"Error : Unable create new process.\n";
        }else if(pid==0){

            signal(SIGTSTP,SIG_DFL);
            signal(SIGCHLD,SIG_DFL);

            std::vector<const char*> argv;
            argv.push_back($1.c_str());
            for(int i=0; i<$2.size(); i++){
                argv.push_back($2[i].c_str());
            }
            argv.push_back(nullptr);

            int ret = execv($1.c_str(),(char *const *)&argv[0]);
            std::cerr<<"Exec Failed with Error Code  "<<errno<<"\n";
            exit(ret);
        }else{
            int status;
            shell.wait_pid = pid;
            waitpid(pid,&status,WUNTRACED);
            shell.waiting = false;
            shell.wait_pid = -1;
        }
    }
}


| _EOF {std::cout<<"\n\n[Shell Terminated By EOF]\n\n";YYACCEPT;};
| error

/* built-in functions */
BUILT_IN :

BG {$$=new bg(shell);}
| CD {$$=new cd(shell);}
| _ECHO {$$=new echo(shell);}
| EXEC {$$=new command(shell);}
| EXIT {std::cout<<"\n[Shell Terminated by Exit]\n\n";YYACCEPT;}
| FG {$$=new fg(shell);}
| SET {$$=new set(shell);}
| SHIFT {$$=new command(shell);}
| TEST {$$=new command(shell);}
| TIME {$$=new command(shell);}
| UNMASK {$$=new command(shell);}
| UNSET {$$=new unset(shell);}


;

BIN : ID
{
    auto paths = shell.get_paths();
    bool found = false;

    //search all search path
    for(auto path : paths){
        if(fs::exists(fs::path(path+"/"+$1))){
            $$=fs::path(path+"/"+$1).string();
            found = true;
            break;
        }
    }


    if(!found){
        std::cerr<<$1<<" command not found.\n";
        $$="";
    }

};
| PATH
{
    bool found = false;

    //execute from path
    if(fs::exists(fs::path($1))){
        $$=fs::path($1).string();
        found = true;
    }

    if(!found){
        std::cerr<<$1<<" : No such file or directory.\n";
        $$="";
    }else{

        auto p = fs::status($1).permissions();
        bool perm_x =((p & fs::perms::owner_exec) != fs::perms::none)|| ((p & fs::perms::group_exec) != fs::perms::none)|| ((p & fs::perms::others_exec) != fs::perms::none);

        if(!perm_x){
            std::cerr<<$1<<" : Permission denied.\n";
            $$ = "";
        }
    }
}

/* arguments */
ARGUMENTS : {$$ = std::vector<std::string>();}
| ARGUMENTS NAME {std::swap($$, $1);$$.push_back($2);};

/* redirection */

REDIRECTION :  /* empty */{$$ = std::vector<std::tuple<int,int,std::string>>();} | REDIRECTION RD_OP NAME

{
    std::swap($$,$1);
    $$.push_back(std::tuple<int,int,std::string>($2.first,$2.second,$3));

};

RD_OP : RD_I {$$ = std::make_pair($1,0);} | RD_O {$$ = std::make_pair($1,1);} | RD_O_AP {$$ = std::make_pair($1,2);};

//resolve name

NAME : VAR
{
    if(shell.env.count($1)){
        $$ = shell.env[$1];
    }else{
        $$ = "";
    }
}
| STR
{
    $$ = $1.substr(1,$1.size()-2);
}
| ID
{
    $$ = $1;
}
| PATH
{
    $$ = $1;
}

//reslove PATHa

PATH : _PATH
{
    $$ = shell.resolve_path($1).string();
}
%%

namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cout << *driver.location_ << ": " << m << "\n";
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
