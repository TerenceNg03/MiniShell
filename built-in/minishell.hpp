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

class minishell{
    friend class command;

private:

    std::list<std::pair<std::thread, std::string>> threads;

public:

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
    void set_args(std::vector<std::string> args);
    void set_is(std::istream& i);
    void set_os(std::ostream& o);
    void set_es(std::ostream& err);
};

class cd:public command{
public:

    cd(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
};

class clr:public command{
public:

    clr(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
};
#endif /* minishell_hpp */
