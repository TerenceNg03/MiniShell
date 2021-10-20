//
//  main.cpp
//  myshell
//
//  Created by Adam Wu on 2021/8/7.
//

#include "driver.hh"
#include "minishell.hpp"
#include <signal.h>

using namespace std;

minishell* shell = nullptr;

struct sigaction sa_ch,sa_sus;

void child_term_handler(int sig, siginfo_t *info, void *context)
{
    pid_t cpid = info->si_pid;
    if(shell&&shell->child_p.count(cpid)){
        string name = shell->child_p[cpid];
        if(!shell->waiting)cout<<"\nProcess Terminated : "<<cpid<<" "<<name<<"\n";
        shell->child_p.erase(cpid);
    }
}

void suspend_handler(int sig, siginfo_t *info, void *context){

    if(shell&&shell->waiting&&shell->wait_pid!=-1){
        string name = shell->child_p[shell->wait_pid];
        shell->waiting = false;
        pid_t tmp = shell->wait_pid;
        shell->wait_pid = -1;
        kill(tmp, SIGTSTP);
        cout<<"\nProcess Suspended : "<<tmp<<" "<<name<<"\n";
    }
}

int main(int argc, const char * argv[]) {

    //setup driver
    parse::Driver driver(vector<string>(argv,argv+argc));
    shell = &driver.shell;

    //setup signal handler

    //child terminated
    sa_ch.sa_flags = SA_RESTART;
    sa_ch.sa_sigaction = child_term_handler;
    sigaction(SIGCHLD, &sa_ch, NULL);

    //suspend children
    sa_sus.sa_flags = SA_RESTART;
    sa_sus.sa_sigaction = suspend_handler;
    sigaction(SIGTSTP, &sa_sus, NULL);

    //run parser
    driver.parse();

    return 0;
}
