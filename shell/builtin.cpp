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


namespace fs = std::filesystem;
using namespace std;

void command::set_args(vector<std::string> args){
    arguments = args;
}

void command::set_is(istream* i){input = i;};
void command::set_os(ostream* o){output = o;};
void command::set_es(ostream* err){error = err;};

int cd::execute(){
    if(arguments.size()<1){
        *error<<"Too few arguments\n";
        return -1;
    }

    fs::path p = shell.resolve_path(arguments.at(0));
    if(fs::is_directory(p)){
        fs::current_path(p);
        shell.env["PWD"] = fs::current_path().string();
    }else{
        *error<<arguments[0]<<" : is not a directory.\n";
    }
    return 0;
}

int bg::execute(){
    if(arguments.size()<1){
        *error<<"Too few arguments\n";
        return -1;
    }

    for(auto n : arguments[0]){
        if(!isdigit(n)){
            *error<<"Invalid pid : "<<arguments[0]<<"\n";
            return -1;
        }
    }

    int _n = atoi(arguments.at(0).c_str());
    int job_n = _n-1;
    job* j = shell.first_job;
    while(j&&job_n){
        j = j->next;
        job_n--;
    }
    if(j){
        shell.put_job_in_background(j, 1);
    }else{
        fprintf(stderr, "No such job %%%d\n",_n);
    }


    return 0;
}

int fg::execute(){
    if(arguments.size()<1){
        *error<<"Too few arguments\n";
        return -1;
    }

    for(auto n : arguments[0]){
        if(!isdigit(n)){
            *error<<"Invalid pid : "<<arguments[0]<<"\n";
            return -1;
        }
    }

    int _n = atoi(arguments.at(0).c_str());
    int job_n = _n-1;
    job* j = shell.first_job;
    while(j&&job_n){
        j = j->next;
        job_n--;
    }
    if(j){
        shell.put_job_in_foreground(j, 1);
    }else{
        fprintf(stderr, "No such job %%%d\n",_n);
    }

    return 0;
}

int set::execute(){

    for(auto p : shell.env){
        *output<<p.first<<" = "<<p.second<<"\n";
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
            *output<<arg.substr(1,arg.size()-1);
        }else{
            *output<<arg;
        }
        if(flag){
            *output<<" ";
            flag = false;
        }
    }
    *output<<"\n";
    return 0;
}

int jobs::execute(){
    if(!shell.first_job)return 0;

    job* j = shell.first_job;
    int i=1;
    while (j) {
        printf("[%%%d] pgid:[%d] %s\n",i,j->pgid,j->command);
        process *p = j->first_process;
        while (p) {
            printf("\tpid:[%d] status:%s\n",p->pid,p->stopped?"Stopped":"Running");
            p = p->next;
        }
        j = j->next;
        i++;
    }
    return 0;;
}
