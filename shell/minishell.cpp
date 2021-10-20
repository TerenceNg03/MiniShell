//
//  minishell.cpp
//  minishell
//
//  Created by Adam Wu on 2021/8/12.
//

#include "minishell.hpp"
#include <regex>
#include <filesystem>
#include <unistd.h>


namespace fs = std::filesystem;
using namespace std;


minishell::minishell(std::vector<std::string> args){
    pid_t pid = getpid();
    child_p[pid] = "minishell";

    env["PWD"] = fs::current_path();
}

