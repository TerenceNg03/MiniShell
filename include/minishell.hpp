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
#include <termios.h>
#include <mutex>
#include "job.hpp"

class parseFlexLexer;

class minishell{

public:

    pid_t shell_pgid;
    struct termios shell_tmodes;
    int shell_terminal;
    int shell_is_interactive;
    /* wait_pid will not return when an interrupt occur and call wait_pid so we need to use a mutex lock to address this issue */
    char wait_flag;
    
    /* The active jobs are linked into a list.  This is its head.   */
    job *first_job = NULL;

    std::map<std::string,std::string> env;
    minishell(std::vector<std::string> args);
    std::filesystem::path resolve_path(std::string s);
    std::vector<std::string> get_paths();

    void init_shell(void);
    void launch_process (process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground);

    /* check out https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html for how to implement a job control shell*/
    void launch_job (job *j, int foreground);
    void put_job_in_foreground (job *j, int cont);
    void put_job_in_background (job *j, int cont);
    void do_job_notification (void);
    void wait_for_job (job *j);
    void update_status (void);
    int mark_process_status (pid_t pid, int status);
    void format_job_info (job *j, const char *status);
    void mark_job_as_running (job *j);
    void continue_job (job *j, int foreground);
    void free_job(job* j);
    job * find_job (pid_t pgid);

    /* Malloc job for parser */
    job* malloc_job(std::string& command, std::vector<std::string>& args, fd3& rds);

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

class jobs:public command{
public:

    jobs(minishell& shell, std::vector<std::string> args =std::vector<std::string>()):command(shell,args){};

    virtual int execute();
    virtual std::string get_name(){return "jobs";};
};
#endif /* minishell_hpp */
