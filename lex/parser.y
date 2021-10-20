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

%token BG CD CLR _ECHO EXEC EXIT FG MAN PS SET SHIFT TEST TIME UNMASK UNSET NEWLINE BACK PIPE UNKNOWN
%token <int> RD_O_AP RD_I RD_O
%token _EOF
%token <std::string> NAME

%type CMDS
%type<command*> BUILT_IN
%type<std::vector<std::tuple<int,int,std::string>>> REDIRECTION
%type<std::vector<std::string>> ARGUMENTS
%type<std::string> BIN
%type<bool> BACKGROUND;
%type<std::pair<int,int>> RD_OP;

%%

    CMDS :  /*empty*/{std::cout<<"\nMyshell By Adam Wu\n\nmyshell "<<fs::path(shell.env["PWD"]).filename().string()<<" $ ";}|CMDS CMD NEWLINE {std::cout<<"myshell "<<fs::path(shell.env["PWD"]).filename().string()<<" $ ";};


CMD : | BUILT_IN ARGUMENTS REDIRECTION BACKGROUND
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

    if(!$4){
        $1->execute();
    }else{
        pid_t pid = fork();
        if(pid==-1){
            std::cerr<<"Error : Unable create new process.\n";
            return -1;
        }else if(pid==0){
            signal(SIGTSTP,SIG_DFL);
            int ret = $1->execute();
            exit(ret);
        }else{
            shell.child_p[pid] = $1->get_name();
        }
    }

//    pid_t pid = fork();
//    if(!$4) shell.waiting = true;
//    if(pid==-1){
//        std::cerr<<"Error : Unable create new process.\n";
//        return -1;
//    }else if(pid==0){
//        signal(SIGTSTP,SIG_DFL);
//        int ret = $1->execute();
//        exit(ret);
//    }else{
//        shell.child_p[pid] = $1->get_name();
//        int status;
//        WEXITSTATUS(status);
//        if(!$4){
//            shell.wait_pid = pid;
//            waitpid(pid,&status,WUNTRACED);
//            shell.waiting = false;
//            shell.wait_pid = -1;
//        }
//    }



    if($1)delete $1;
    if(i)delete i;
    if(o)delete o;
}

| BIN ARGUMENTS REDIRECTION BACKGROUND
{
    if($1!=""){
        pid_t pid = fork();
        if(!$4) shell.waiting = true;

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

            int ret = execvp($1.c_str(),(char *const *)&argv[0]);

            exit(ret);
        }else{
            shell.child_p[pid] = fs::path($1).filename().string();
            int status;
            WEXITSTATUS(status);
            if(!$4){
                shell.wait_pid = pid;
                waitpid(pid,&status,WUNTRACED);
                shell.waiting = false;
                shell.wait_pid = -1;
            }
        }
    }
}

| _EOF {std::cout<<"\n\n[Process Terminated By EOF]\n\n";YYACCEPT;};
| error

/* built-in functions */
BUILT_IN :

BG {$$=new bg(shell);}
| CD {$$=new cd(shell);}
| CLR {$$=new clr(shell);}
| _ECHO {$$=new command(shell);}
| EXEC {$$=new command(shell);}
| EXIT {std::cout<<"\n[Process Quited]\n\n";YYACCEPT;}
| FG {$$=new fg(shell);}
| MAN {$$=new command(shell);}
| PS {$$=new ps(shell);}
| SET {$$=new command(shell);}
| SHIFT {$$=new command(shell);}
| TEST {$$=new command(shell);}
| TIME {$$=new command(shell);}
| UNMASK {$$=new command(shell);}
| UNSET {$$=new command(shell);}


;

BIN : NAME
{
    auto paths = shell.get_paths();
    bool found = false;

    for(auto path : paths){
        if(fs::exists(fs::path(path+"/"+$1))){
            $$=path+"/"+$1;
            found = true;
            break;
        }
    }

    if(!found){
        std::cout<<$1<<" command not found.\n";
        $$="";
    }

};

/* arguments */
ARGUMENTS : {$$ = std::vector<std::string>();} | ARGUMENTS NAME {std::swap($$, $1);$$.push_back($2);};

/* redirection */

REDIRECTION :  /* empty */{$$ = std::vector<std::tuple<int,int,std::string>>();} | REDIRECTION RD_OP NAME

{
    std::swap($$,$1);
    $$.push_back(std::tuple<int,int,std::string>($2.first,$2.second,$3));

};

RD_OP : RD_I {$$ = std::make_pair($1,0);} | RD_O {$$ = std::make_pair($1,1);} | RD_O_AP {$$ = std::make_pair($1,2);};

/* background */

BACKGROUND : {$$ = false;} | BACK {$$ = true;};

%%

namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cout << *driver.location_ << ": " << m << "\n";
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
