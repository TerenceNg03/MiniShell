//
//  job.hpp
//  MiniShell
//
//  Created by Terence Ng on 2021/11/6.
//

#ifndef job_h
#define job_h

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <vector>
#include <list>

class minishell;
namespace shellcmd {
    class command;
}

typedef struct fd3{
    int _in;
    int _out;
    int _err;
    int fail;
}fd3;

/* A process is a single process.  */
struct process
{
    //struct process *next;       /* next process in pipeline */
    std::vector<std::string> args;                /* for exec */
    pid_t pid;                  /* process ID */
    bool completed = false;             /* true if process has completed */
    bool stopped = false;               /* true if process has stopped */
    int status;                 /* reported status value */

    char is_builtin;
    std::shared_ptr<shellcmd::command> cmd;

    process(std::string& command, std::vector<std::string>& args);
    process(std::shared_ptr<shellcmd::command> cmd);
};

/* A job is a pipeline of processes.  */
struct job
{
    char foreground;           /* launch at foreground */
    std::string command;              /* command line, used for messages */
    std::list<process> processes;/* list of processes in this job */
    pid_t pgid;                 /* process group ID */
    char notified;              /* true if user told about stopped job */
    struct termios tmodes;      /* saved terminal modes */
    int stdin, stdout, stderr;  /* standard i/o channels */

    job(std::string& cmd, fd3& rds, char foreground=1);

    int is_stopped();
    int is_completed();
};


#endif /* job_h */
