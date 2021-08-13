//
//  minishell.cpp
//  minishell
//
//  Created by Adam Wu on 2021/8/12.
//

#include "minishell.hpp"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

void command::set_args(vector<std::string> args){
    arguments = args;
}

void command::set_is(istream* i){input = i;};
void command::set_os(ostream* o){output = o;};
void command::set_es(ostream* err){error = err;};

minishell::minishell(vector<std::string> args){};

int cd::execute(){
    if(arguments.size()<1){
        *error<<"Too few arguments\n";
        return -1;
    }
    if(fs::is_directory(filesystem::current_path().string()+'/'+arguments.at(0))){
        fs::current_path(filesystem::current_path().string()+'/'+arguments.at(0));
    }else{
        *error<<arguments[0]<<" : is not a directory.\n";
    }
    return 0;
}

int clr::execute(){
    *output<<"\033[2J\033[1;1H";
    return 0;
}

int dir::execute(){
    fs::directory_iterator dit(fs::current_path());
    for (auto f:dit){
        *output<<f.path().filename().string()<<'\t';
    }
    *output<<"\n";
    return 0;
}
