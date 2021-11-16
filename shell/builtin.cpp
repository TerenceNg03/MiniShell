//
//  builtin.cpp
//  MiniShell
//
//  Created by Terence Ng on 2021/10/20.
//

#include "minishell.hpp"
#include <regex>
#include <filesystem>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdlib.h>


namespace fs = std::filesystem;
using namespace std;
using namespace shellcmd;

void command::set_args(vector<std::string> args){
    arguments = args;
}

int cd::execute(){
    if(arguments.size()<1){
        return -1;
    }

    fs::path p = shell.resolve_path(arguments.at(0));
    if(fs::is_directory(p)){
        fs::current_path(p);
        shell.env["PWD"] = fs::current_path().string();
    }else{
        fprintf(stderr, "cd: %s : is not a directory.\n",arguments[0].c_str());
    }
    return 0;
}

int bg::execute(){
    if(arguments.size()<1){
        fprintf(stderr, "bg: no current job\n");
        return -1;
    }

    for(auto n : arguments[0]){
        if(!isdigit(n)){
            fprintf(stderr, "bg: Invalid pid : %s\n", arguments[0].c_str());
            return -1;
        }
    }

    int _n = atoi(arguments.at(0).c_str());
    int job_n = _n-1;
    auto j = shell.jobs.begin();
    while(j != shell.jobs.end() && job_n){
        j = next(j);
        job_n--;
    }
    if(j != shell.jobs.end()){
        shell.put_job_in_background(*j, 1);
    }else{
        fprintf(stderr, "bg: No such job %%%d\n",_n);
    }


    return 0;
}

int fg::execute(){
    if(arguments.size()<1){
        fprintf(stderr, "fg: no current job\n");
        return -1;
    }

    for(auto n : arguments[0]){
        if(!isdigit(n)){
            fprintf(stderr, "fg: Invalid pid : %s\n", arguments[0].c_str());
            return -1;
        }
    }

    int _n = atoi(arguments.at(0).c_str());
    int job_n = _n-1;
    auto j = shell.jobs.begin();
    while(j != shell.jobs.end() && job_n){
        j = next(j);
        job_n--;
    }
    if(j!=shell.jobs.end()){
        shell.put_job_in_foreground(*j, 1);
    }else{
        fprintf(stderr, "fg: No such job %%%d\n",_n);
    }

    return 0;
}

int set::execute(){

    for(auto p : shell.env){
        printf("%s = %s\n",p.first.c_str(),p.second.c_str());
    }
    return 0;
}

int unset::execute(){
    if(arguments.size()<1){
        return 0;
    }
    shell.env.erase(shell.env.find(arguments.at(0)));
    return 0;
}


int echo::execute(){
    regex re("[\'\"].*[\'\"]");
    bool flag = true;
    for(auto arg : arguments){
        if(regex_match(arg, re)){
            printf("%s",arg.substr(1,arg.size()-1).c_str());
        }else{
            printf("%s",arg.c_str());
        }
        if(flag){
            putchar(' ');
            flag = false;
        }
    }
    putchar('\n');
    return 0;
}

int exit::execute(){
    std::exit(0);
    return 0;
}

int jobs::execute(){

    auto j = shell.jobs.begin();
    int i=1;
    while (j!=shell.jobs.end()) {
        printf("[%%%d] pgid:[%d] %s\n",i,j->pgid,j->command.c_str());
        auto p = j->processes.begin();
        while (p != j->processes.end()) {
            printf("\tpid:[%d] status:%s\n",p->pid,p->stopped?"Stopped":"Running");
            p = next(p);
        }
        j = next(j);
        i++;
    }
    return 0;;
}
