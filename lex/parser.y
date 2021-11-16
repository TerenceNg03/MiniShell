%skeleton "lalr1.cc"

%{
#include <stdio.h>
#include <string>
#include "y.tab.hh"
#include "scanner.hh"
#define yylex driver.scanner_->lex
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
%lex-param {Driver &driver}{minishell& shell}
%parse-param {Driver &driver}{minishell& shell}
%define parse.error verbose
%language "c++"
%define api.value.type variant
%define api.token.constructor

%token <std::string> ILLEGAL
%token NEWLINE BACK PIPE ASSIGN GREATER LESSER RIGHT_SHIFT LEFT_SHIFT SEMICOLON L_BRACKET R_BRACKET
%token <int> NUM NUM_GREATER NUM_LESSER NUM_RIGHT_SHIFT
%token <std::string> VAR STR ID _PATH
%token END

%type CMDS
%type <std::string> WORD PATH
%type <fd3> REDIRECTION SIMPLE_RD
%type <std::vector<std::string>> ARGUMENTS
%type <std::string> BIN
%type <std::optional<job>> SIMPLE_CMD PIPELINE
%type <std::list<std::optional<job>>> CMD STATEMENT

%%

CMDS :  /*empty*/
{
    printf("minishell %s $ ",fs::path(shell.env["PWD"]).filename().string().c_str());

}
|CMDS STATEMENT NEWLINE
{
    char ok = 1;
    for(auto j: $2){
        if(!j)ok=0;
    }

    if(ok)for(auto j: $2){
        shell.jobs.push_back(*j);
        shell.launch_job(shell.jobs.back());
    }

    driver.location_->lines();
    driver.location_->step();
    driver.scanner_->reset_current_col();
    printf("minishell %s $ ",fs::path(shell.env["PWD"]).filename().string().c_str());

}
| CMDS error NEWLINE
{
    driver.location_->lines();
    driver.location_->step();
    driver.scanner_->reset_current_col();
    yyerrok;
    printf("minishell %s $ ",fs::path(shell.env["PWD"]).filename().string().c_str());
}

SIMPLE_CMD : BIN ARGUMENTS REDIRECTION
{
    auto p = shell.resolve_command($1,$2);
    if(p){
        job j($1, $3);
        j.processes.push_front(*p);
        $$ = j;
    }else{
        $$ = {};
    }

}

STATEMENT: CMD
{
    $$ = $1;
}

CMD :
/* empty command */
{
    $$.clear();
}
| SEMICOLON
{
    $$.clear();
}
| SIMPLE_CMD SEMICOLON CMD
{
    $3.push_front($1);
    $$ = $3;
}
| SIMPLE_CMD BACK CMD
{
    if($1)$1->foreground = 0;
    $3.push_front($1);
    $$ = $3;
}
| SIMPLE_CMD
{
    $$.clear();
    $$.push_front($1);
}
| END
{
    puts("\n\n[ Shell EXITED ]\n");YYACCEPT;
}
| PIPELINE REDIRECTION BACK CMD
{
    if($1){
        $1->foreground = 0;
        $1->stdin = ($2._in<0)?0:$2._in;
        $1->stdout = ($2._out<0)?1:$2._out;
        $1->stderr = ($2._err<0)?2:$2._err;
    }
    $4.push_front($1);
    $$ = $4;
}

| PIPELINE REDIRECTION SEMICOLON CMD
{
    if($1){
        $1->foreground = 1;
        $1->stdin = ($2._in<0)?0:$2._in;
        $1->stdout = ($2._out<0)?1:$2._out;
        $1->stderr = ($2._err<0)?2:$2._err;
    }
    $4.push_front($1);
    $$ = $4;
}
| PIPELINE REDIRECTION
{
    if($1){
        $1->foreground = 1;
        $1->stdin = ($2._in<0)?0:$2._in;
        $1->stdout = ($2._out<0)?1:$2._out;
        $1->stderr = ($2._err<0)?2:$2._err;
    }
    $$.clear();
    $$.push_front($1);
}


PIPELINE :
BIN ARGUMENTS PIPE BIN ARGUMENTS
{
    auto p1 = shell.resolve_command($1,$2);
    auto p2 = shell.resolve_command($4,$5);
    fd3 fd = {0};
    if(p1&&p2){
        job j($1,fd);
        j.processes.push_front(*p2);
        j.processes.push_front(*p1);
        $$ = j;
    }else{
        $$ = {};
    }

}

| PIPELINE PIPE BIN ARGUMENTS
{
    auto p = shell.resolve_command($3,$4);
    if(p&&$1){
        $1->processes.push_back(*p);
        $$ = $1;
    }else{
        $$ = {};
    }
}




BIN : ID
{
    $$ = shell.resolve_id($1);

};
| PATH
{
    $$ = $1;
}


/* arguments */
ARGUMENTS : {$$ = std::vector<std::string>();}
| ARGUMENTS WORD {std::swap($$, $1);$$.push_back($2);};

/* redirection */

REDIRECTION :
/* empty */
{
    $$._in=$$._out=$$._err=-1;
}
| REDIRECTION SIMPLE_RD
{
    $1._in = ($2._in<0)?$1._in:$2._in;
    $1._out = ($2._out<0)?$1._out:$2._out;
    $1._err = ($2._err<0)?$1._err:$2._err;
    $1.fail = $2.fail|$1.fail;
    $$ = $1;
}

SIMPLE_RD : NUM_GREATER BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
            break;
        case 1:
            _fd3._out = $3;
            break;
        case 2:
            _fd3._err = $3;
            break;
        default:
            fprintf(stderr,"Redirection Error : Cannot redirect file descriptor %d\n",$1);
            _fd3.fail = 1;
    }
    $$ = _fd3;
}
|NUM_LESSER BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
            _fd3._in = $3;
            break;
        case 1:
        case 2:
            break;
        default:
        fprintf(stderr,"shell : Bad file descriptor %d\n",$1);
        _fd3.fail = 1;
    }
    $$ = _fd3;
}
| NUM_RIGHT_SHIFT BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
            break;
        case 1:
            _fd3._out = $3;
            break;
        case 2:
            _fd3._err = $3;
            break;
        default:
        fprintf(stderr,"shell : Bad file descriptor %d\n",$1);
        _fd3.fail = 1;
    }
    $$ = _fd3;
}
| NUM_GREATER WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
            break;
        case 1:
            _fd3._out = open($2.c_str(),O_CREAT|O_WRONLY,0666);
            if(_fd3._out<0){
                fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
                _fd3.fail = 1;
            }
            break;
        case 2:
            _fd3._err = open($2.c_str(),O_CREAT|O_WRONLY,0666);
            if(_fd3._err<0){
                fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
                _fd3.fail = 1;
            }
            break;
        default:
        fprintf(stderr,"shell : Bad file descriptor %d\n",$1);
        _fd3.fail = 1;
    }
    $$ = _fd3;
}
| NUM_LESSER WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
        if((_fd3._in=open($2.c_str(),O_CREAT|O_RDONLY,0666))<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
            _fd3.fail = 1;
        }
        case 1:
        case 2:
            break;
        default:
        fprintf(stderr,"shell : Bad file descriptor %d\n",$1);
        _fd3.fail = 1;
    }
    $$=_fd3;
}
| NUM_RIGHT_SHIFT WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    switch($1){
        case 0:
        break;
        case 1:
        _fd3._out = open($2.c_str(),O_CREAT | O_APPEND|O_WRONLY,0666);
        if(_fd3._out<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
            _fd3.fail = 1;
        }
        break;
        case 2:
        _fd3._err = open($2.c_str(),O_CREAT | O_APPEND|O_WRONLY,0666);
        if(_fd3._err<0){
            fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
            _fd3.fail = 1;
        }
        break;
        default:
        fprintf(stderr,"shell : Bad file descriptor %d\n",$1);
        _fd3.fail = 1;
    }
    $$ = _fd3;
}
| GREATER BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    _fd3._in = $3;
    if($3>2){fprintf(stderr,"shell : Bad file descriptor %d\n",$3),_fd3.fail = 1;}
    $$ = _fd3;
}
| LESSER BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    _fd3._in = $3;
    if($3>2){fprintf(stderr,"shell : Bad file descriptor %d\n",$3),_fd3.fail = 1;}
    $$ = _fd3;
}
| RIGHT_SHIFT BACK NUM
{
    fd3 _fd3 = {-1,-1,-1,0};
    _fd3._out = $3;
    if($3>2){fprintf(stderr,"shell : Bad file descriptor %d\n",$3),_fd3.fail = 1;}
    $$ = _fd3;
}
| GREATER WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    _fd3._out = open($2.c_str(),O_CREAT|O_WRONLY,0666);
    if(_fd3._out<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
        _fd3.fail = 1;
    }
    $$ = _fd3;
}
| LESSER WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    if((_fd3._in=open($2.c_str(),O_CREAT| O_RDONLY,0666))<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
        _fd3.fail = 1;
    }
    $$=_fd3;
}
| RIGHT_SHIFT WORD
{
    fd3 _fd3 = {-1,-1,-1,0};
    _fd3._out = open($2.c_str(),O_CREAT | O_APPEND | O_WRONLY,0666);
    if(_fd3._out<0){
        fprintf(stderr,"Redirection Error : Failed to open %s\n",$2.c_str());
        _fd3.fail = 1;
    }
    $$ = _fd3;
}

//resolve name

WORD : VAR
{
    if(shell.env.count($1)){
        $$ = shell.env[$1];
    }else{
        $$ = "";
    }
}
| STR
{
    $$ = shell.resolve_str($1);
}
| ID
{
    $$ = shell.resolve_id($1);
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
    void Parser::error(const location& loc, const std::string& m)
    {
        size_t current_col = driver.scanner_->current_col;
        std::cout << "line " << loc << ": " << m << "\n";
        fprintf(stderr,"\t%s\t", driver.scanner_->current_line.c_str());
        for(int i = 0; i < current_col; i++)
        fprintf(stderr,"~");
        fprintf(stderr,"^\n");
    }
    
}
