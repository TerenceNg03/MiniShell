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
#include <filesystem>

class minishell{

public:
    std::map<std::string,std::string> env;
    bool waiting;
    pid_t wait_pid=-1;

    minishell(std::vector<std::string> args);
    std::filesystem::path resolve_path(std::string s);
    std::vector<std::string> get_paths();
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
    virtual ~command(){};
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

class set:public command{
public:

    set(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "set";};
};

class unset:public command{
public:

    unset(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "unset";};
};

class echo:public command{
public:

    echo(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "echo";};
};
#endif /* minishell_hpp */
