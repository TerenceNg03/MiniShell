//
//  minishell.hpp
//  minishell
//
//  Created by Adam Wu on 2021/8/12.
//

#ifndef minishell_hpp
#define minishell_hpp

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <list>
#include <ostream>
#include "process.hpp"

class minishell{
    friend class command;

private:

public:
    std::map<pid_t, std::string> child_p;

    minishell(std::vector<std::string> args);
};

class command{

protected:
    std::vector<std::string> arguments;
    std::istream* input = &std::cin;
    std::ostream* output = &std::cout;
    std::ostream* error = &std::cerr;
    minishell* shell;
public:
    command(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):shell(&shell),arguments(args){};
    virtual int execute(){*error<<"Empty command executed.\n";return -1;};
    virtual std::string get_name(){return "Empty command";};
    void set_args(std::vector<std::string> args);
    void set_is(std::istream* i);
    void set_os(std::ostream* o);
    void set_es(std::ostream* err);
};

class cd:public command{
public:

    cd(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "cd";};
};

class clr:public command{
public:

    clr(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "clear";};
};

class ls:public command{
public:

    ls(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "ls";};
};

class sleep:public command{
public:

    sleep(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "sleep";};
};

class ps:public command{
public:

    ps(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "ps";};
};
#endif /* minishell_hpp */
