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
#include <list>
#include <ostream>
#include <map>

class minishell{

public:
    std::map<std::string,std::string> env;
    std::map<pid_t, std::string> child_p;
    bool waiting;
    pid_t wait_pid=-1;
    minishell(std::vector<std::string> args);
};

class command{

protected:
    minishell& shell;
    std::vector<std::string> arguments;
    std::istream* input = &std::cin;
    std::ostream* output = &std::cout;
    std::ostream* error = &std::cerr;
public:
    command(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):shell(shell),arguments(args){};
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

class bg:public command{
public:

    bg(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "bg";};
};

class fg:public command{
public:

    fg(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "fg";};
};
#endif /* minishell_hpp */
