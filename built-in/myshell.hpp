//
//  myshell.hpp
//  myshell
//
//  Created by Adam Wu on 2021/8/12.
//

#ifndef myshell_hpp
#define myshell_hpp

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <list>

class command{

private:
    std::vector<std::string> arguments;
    std::istream* input;
    std::ostream* output;
    std::ostream* error;
public:
    command(std::istream& i = std::cin, std::ostream& o = std::cout, std::ostream& err = std::cerr, std::vector<std::string> args =std::vector<std::string>()):arguments(args),input(&i),output(&o),error(&err){};
    virtual int execute(){return -1;};
    void set_args(std::vector<std::string> args);
    void set_is(std::istream& i);
    void set_os(std::ostream& o);
    void set_es(std::ostream& err);
};

class myshell{
    friend class command;

private:

    std::list<std::pair<std::thread, std::string>> threads;

public:

    myshell(std::vector<std::string> args);
};
#endif /* myshell_hpp */
