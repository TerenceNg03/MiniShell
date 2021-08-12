//
//  myshell.cpp
//  myshell
//
//  Created by Adam Wu on 2021/8/12.
//

#include "myshell.hpp"

void command::set_args(std::vector<std::string> args){
    arguments = args;
}

void command::set_is(std::istream& i){input = &i;};
void command::set_os(std::ostream& o){output = &o;};
void command::set_es(std::ostream& err){error = &err;};
myshell::myshell(std::vector<std::string> args){};
