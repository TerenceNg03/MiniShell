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

int ps::execute(){
    *output<<"\tPID\tName\n";
    for(auto p : shell.child_p){
        *output<<"\t"<<p.first<<"\t"<<p.second<<"\n";
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

    pid_t cpid = atoi(arguments.at(0).c_str());

    if(shell.child_p.find(cpid)!=shell.child_p.end()){
        kill(cpid, SIGCONT);
    }else{
        *error<<"Invalid pid : "<<arguments[0]<<"\n";
        return -2;
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

    pid_t cpid = atoi(arguments.at(0).c_str());

    if(shell.child_p.find(cpid)!=shell.child_p.end()){
        shell.waiting = true;
        shell.wait_pid = cpid;
        kill(cpid, SIGCONT);
        int status;
        waitpid(cpid, &status, WUNTRACED);
    }else{
        *error<<"Invalid pid : "<<arguments[0]<<"\n";
        return -2;
    }
    return 0;
}
