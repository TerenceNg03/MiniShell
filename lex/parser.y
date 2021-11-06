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
    #include "job.hpp"

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

%token BG CD _ECHO EXEC EXIT FG SET SHIFT TEST TIME UNMASK UNSET JOBS UNKNOWN
%token NEWLINE BACK PIPE ASSIGN GREATER LESSER RIGHT_SHIFT LEFT_SHIFT SEMICOLON
%token <int> NUM
%token _EOF END
%token <std::string> VAR STR ID _PATH

%type CMDS
%type <std::string> NAME PATH
%type <command*> BUILT_IN
%type <fd3> REDIRECTION
%type <std::vector<std::string>> ARGUMENTS
%type <std::string> BIN

%%

CMDS :  /*empty*/
{
    printf("minishell %s $ ",fs::path(shell.env["PWD"]).filename().string().c_str());

}
|CMDS CMD NEWLINE
{

    printf("minishell %s $ ",fs::path(shell.env["PWD"]).filename().string().c_str());

};


CMD :/* empty command */
| CMD SEMICOLON CMD

| BUILT_IN ARGUMENTS REDIRECTION BACK CMD
{
    $1->set_args($2);
    pid_t pid = fork();
    if(pid==-1){
        perror("fork");
        return -1;
    }else if(pid==0){
        signal(SIGTSTP,SIG_DFL);
        int ret = $1->execute();
        exit(ret);
    }
    if($1)delete $1;
    
}

|BUILT_IN ARGUMENTS REDIRECTION
{
    $1->set_args($2);

    $1->execute();

    if($1)delete $1;
}


| BIN ARGUMENTS REDIRECTION BACK CMD
{
    if($1!=""){
        job* j = shell.malloc_job($1,$2,$3);
        if(j){
            shell.launch_job(j,0);
        }
    }

}

| BIN ARGUMENTS REDIRECTION
{
    if($1!=""){
        job* j = shell.malloc_job($1,$2,$3);
        if(j){
            shell.launch_job(j,1);
        }
    }
}
| error

/* built-in functions */
BUILT_IN :

BG {$$=new bg(shell);}
| CD {$$=new cd(shell);}
| _ECHO {$$=new echo(shell);}
| EXEC {$$=new command(shell);}
| EXIT {std::cout<<"\n\n[ Shell EXITED ]\n\n";YYACCEPT;}
| FG {$$=new fg(shell);}
| SET {$$=new set(shell);}
| SHIFT {$$=new command(shell);}
| TEST {$$=new command(shell);}
| TIME {$$=new command(shell);}
| UNSET {$$=new unset(shell);}
| JOBS {$$=new jobs(shell);}
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

REDIRECTION :
/* empty */ {
    $$._in=$$._out=$$._err=-1;
}
| NUM GREATER BACK NUM REDIRECTION
{
    switch($1){
        case 0:
            break;
        case 1:
            $5._out = $4;
            break;
        case 2:
            $5._err = $4;
            break;
        default:
            fprintf(stderr,"Redirection Error : Cannot redirect file descriptor %d\n",$1);
    }
    $$ = $5;
}
|NUM LESSER BACK NUM REDIRECTION
{
    switch($1){
        case 0:
            $5._in = $4;
            break;
        case 1:
        case 2:
            break;
        default:
        fprintf(stderr,"Redirection Error : Cannit redirect file descriptor %d\n",$1);
    }
    $$ = $5;
}
| NUM RIGHT_SHIFT BACK NUM REDIRECTION
{
    switch($1){
        case 0:
            break;
        case 1:
            $5._out = $4;
            break;
        case 2:
            $5._err = $4;
            break;
        default:
        fprintf(stderr,"Redirection Error : Cannit redirect file descriptor %d\n",$1);
    }
    $$ = $5;
}
| NUM GREATER NAME REDIRECTION
{
    switch($1){
        case 0:
            break;
        case 1:
            $4._out = open($3.c_str(),O_CREAT,0777);
            if($4._out<0){
                fprintf(stderr,"Redirection Error : Failed to open %s\n",$3.c_str());
            }
            break;
        case 2:
            $4._err = open($3.c_str(),O_CREAT,0777);
            if($4._err<0){
                fprintf(stderr,"Redirection Error : Failed to open %s\n",$3.c_str());
            }
            break;
        default:
        fprintf(stderr,"Redirection Error : Cannit redirect file descriptor %d\n",$1);
    }
    $$ = $4;
}
| NUM LESSER NAME REDIRECTION
{
    switch($1){
        case 0:
        if(($4._in=open($3.c_str(),O_CREAT,0777))<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$3.c_str());
        }
        case 1:
        case 2:
            break;
        default:
        fprintf(stderr,"Redirection Error : Cannit redirect file descriptor %d\n",$1);
    }
    $$=$4;
}
| NUM RIGHT_SHIFT NAME REDIRECTION
{
    switch($1){
        case 0:
        break;
        case 1:
        $4._out = open($3.c_str(),O_APPEND);
        if($4._out<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$3.c_str());
        }
        break;
        case 2:
        $4._err = open($3.c_str(),O_APPEND);
        if($4._err<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$3.c_str());
        }
        break;
        default:
        fprintf(stderr,"Redirection Error : Cannit redirect file descriptor %d\n",$1);
    }
    $$ = $4;
}
| GREATER BACK NUM REDIRECTION
{
    $4._in = $3;
    $$ = $4;
}
| LESSER BACK NUM REDIRECTION
{
    $4._in = $3;
    $$ = $4;
}
| RIGHT_SHIFT BACK NUM REDIRECTION
{
    $4._out = $3;
    $$ = $4;
}
| GREATER NAME REDIRECTION
{
    $3._out = open($2.c_str(),O_CREAT,0777);
    if($3._out<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
    }
    $$ = $3;
}
| LESSER NAME REDIRECTION
{
    if(($3._in=open($2.c_str(),O_CREAT,0777))<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
    }
    $$=$3;
}
| RIGHT_SHIFT NAME REDIRECTION
{
    $3._out = open($2.c_str(),O_APPEND);
    if($3._out<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
    }
    $$ = $3;
}

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
| NUM
{
    $$ = std::to_string($1);
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
