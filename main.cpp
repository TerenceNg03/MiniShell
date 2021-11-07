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

void child_term_handler(int sig)
{
    /* Do not wait if shell is already waiting */
    if(shell&&!shell->wait_flag)shell->do_job_notification();
}

int main(int argc, const char * argv[]) {
//    std::ios::sync_with_stdio(false);

    //setup driver
    parse::Driver driver(vector<string>(argv,argv+argc));
    shell = &driver.shell;

    //setup signal handler

    //child terminated
    signal(SIGCHLD, child_term_handler);

    //run parser
    driver.parse();

    return 0;
}
