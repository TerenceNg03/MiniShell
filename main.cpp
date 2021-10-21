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

struct sigaction sa_ch,sa_sus,sa_int;

bool interrupted = false;

void child_term_handler(int sig, siginfo_t *info, void *context)
{
    pid_t cpid = info->si_pid;

    if(shell&&!shell->waiting)cout<<"\n[ Process Done : "<<cpid<<" ]\n";

    cout<<"\b\b";
}

void suspend_handler(int sig, siginfo_t *info, void *context){

    if (shell&&shell->wait_pid!=-1) {
        //shell->waiting = false;
        pid_t tmp = shell->wait_pid;
        //shell->wait_pid = -1;
        kill(tmp, SIGTSTP);
        cout<<"\n[ Process Suspended : "<<tmp<<" ]\n";
    }
    cout<<"\b\b";
}

void interrupt_handler(int sig, siginfo_t *info, void *context){

    if (shell&&shell->wait_pid!=-1) {
        //shell->waiting = false;
        pid_t tmp = shell->wait_pid;
        //shell->wait_pid = -1;
        kill(tmp, SIGINT);
        cout<<"\n[ Process Terminated : "<<tmp<<" ]\n";
    }
    cout<<"\b\b";

    interrupted = true;

}


int main(int argc, const char * argv[]) {
//    std::ios::sync_with_stdio(false);

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

    //interrupt child
    sa_ch.sa_flags = SA_RESTART;
    sa_int.sa_sigaction = interrupt_handler;
    sigaction(SIGINT, &sa_int, NULL);

    //run parser
    driver.parse();

    return 0;
}
