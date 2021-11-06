//
//  sig_test.cpp
//  minishell
//
//  Created by Terence Ng on 2021/10/21.
//

#include <iostream>
#include <signal.h>
#include <chrono>
#include <ctime>

using namespace std;


struct sigaction sa_ch,sa_sus,sa_int;

void child_term_handler(int sig, siginfo_t *info, void *context){

    auto time = chrono::system_clock::now();
    auto time_c = chrono::system_clock::to_time_t(time);

    cout<<"SIGCHLD received : "<<ctime(&time_c)<<"\n";

}

void suspend_handler(int sig, siginfo_t *info, void *context){

    auto time = chrono::system_clock::now();
    auto time_c = chrono::system_clock::to_time_t(time);

    cout<<"SIGTSTP received : "<<ctime(&time_c)<<"\n";

}

void interrupt_handler(int sig, siginfo_t *info, void *context){

    auto time = chrono::system_clock::now();
    auto time_c = chrono::system_clock::to_time_t(time);

    cout<<"SIGINT received : "<<ctime(&time_c)<<"\n";

}


int main(int argc, const char * argv[]) {

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
    sa_int.sa_flags = SA_RESTART;
    sa_int.sa_sigaction = interrupt_handler;
    sigaction(SIGINT, &sa_int, NULL);

    while((getchar())!=EOF){};
    
    return 0;
}
