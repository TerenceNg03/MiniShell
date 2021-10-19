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

void child_term_handler(int sig, siginfo_t *info, void *context)
{
    pid_t cpid = info->si_pid;
    if(shell&&shell->child_p.count(cpid)){
        string name = shell->child_p[cpid];
        cout<<"\nProcess Terminated : "<<cpid<<" "<<name<<"\n";
        shell->child_p.erase(cpid);
    }
}


int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);

    //setup driver
    parse::Driver driver(vector<string>(argv,argv+argc));
    shell = &driver.shell;

    //setup signal handler
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_sigaction = child_term_handler;
    sigaction(SIGCHLD, &sa, NULL);

    //run parser
    driver.parse();

    return 0;
}
