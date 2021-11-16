//
//  job.c
//  MiniShell
//
//  Created by Terence Ng on 2021/11/6.
//

#include "job.hpp"
#include "minishell.hpp"

using namespace std;

job::job(std::string& cmd, fd3& rds, char foreground){

    /* Set job command */
    command = cmd;
    pgid = 0;
    notified = 0;

    /* Set pipeline */
    this->stdin = (rds._in<0)?0:rds._in;
    this->stdout = (rds._out<0)?1:rds._out;
    this->stderr = (rds._err<0)?2:rds._err;

    this->foreground = foreground;

}


/* Return true if all processes in the job have stopped or completed.  */
int job::is_stopped ()
{
    for (auto p: processes)
        if (!p.completed && !p.stopped)
            return 0;
    return 1;
}

/* Return true if all processes in the job have completed.  */
int job::is_completed ()
{
    for (auto p: processes){
        if (!p.completed)return 0;
    }

    return 1;
}

process::process(std::string& command, std::vector<std::string>& args){

    is_builtin = 0;
    cmd = NULL;
    /* Initilize process */
    this->args = args;
    this->args.insert(this->args.begin(), command);

}

process::process(std::shared_ptr<shellcmd::command> builtin){
    cmd = builtin;
    is_builtin = 1;
}
