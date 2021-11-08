//
//  minicpp
//  minishell
//
//  Created by Adam Wu on 2021/8/12.
//

#include "minishell.hpp"
#include <regex>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>


namespace fs = std::filesystem;
using namespace std;


minishell::minishell(std::vector<std::string> args){

    env["PWD"] = fs::current_path();
    env["PATH"] = fs::current_path().string()+"/bin:/bin:/usr/bin";
    env["HOME"] = getpwuid(getuid())->pw_dir;
    env["SHELL"] = getpwuid(getuid())->pw_shell;
    wait_flag = 0;
    init_shell();
    printf("\nMinishell By Terence Ng\n");
    printf("Shell initilized.\nInteractive : %s\n\n",(shell_is_interactive)?"yes":"no");
}

void minishell::init_shell (void){

    /* See if we are running interactively.  */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty (shell_terminal);

    if (shell_is_interactive)
    {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
            kill (- shell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
        signal (SIGTSTP, SIG_IGN);
        signal (SIGTTIN, SIG_IGN);
        signal (SIGTTOU, SIG_IGN);
        signal (SIGCHLD, SIG_IGN);

        /* Put ourselves in our own process group.  */
        shell_pgid = getpid ();
        if (setpgid (shell_pgid, shell_pgid) < 0)
        {
            perror ("Couldn't put the shell in its own process group");
            exit (1);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp (shell_terminal, shell_pgid);

        /* Save default terminal attributes for   */
        tcgetattr (shell_terminal, &shell_tmodes);
    }
}

vector<string> minishell::get_paths(){
    string paths = env["PATH"];
    regex re(":");
    std::sregex_token_iterator first{paths.begin(), paths.end(), re, -1},
    last;
    return {first, last};
}

std::filesystem::path minishell::resolve_path(std::string s){
    if(s.size()>0&&s[0]=='~'){
        s = env["HOME"] + s.substr(1,s.size());
    }
    return fs::path(s);
}

void minishell::launch_process (process *p, pid_t pgid,
                int infile, int outfile, int errfile,
                int foreground){
    pid_t pid;

    if (shell_is_interactive)
    {
        /* Put the process into the process group and give the process group
         the terminal, if appropriate.
         This has to be done both by the shell and in the individual
         child processes because of potential race conditions.  */
        pid = getpid ();
        if (pgid == 0) pgid = pid;
        setpgid (pid, pgid);
        if (foreground)
            tcsetpgrp (shell_terminal, pgid);

        /* Set the handling for job control signals back to the default.  */
        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);
    }

    /* Set the standard input/output channels of the new process.  */
    if (infile != STDIN_FILENO)
    {
        dup2 (infile, STDIN_FILENO);
        close (infile);
    }
    if (outfile != STDOUT_FILENO)
    {
        dup2 (outfile, STDOUT_FILENO);
        close (outfile);
    }
    if (errfile != STDERR_FILENO)
    {
        dup2 (errfile, STDERR_FILENO);
        close (errfile);
    }

    /* Exec the new process.  Make sure we exit.  */
    execvp (p->argv[0], p->argv);
    perror ("execvp");
    exit (1);
}

void minishell::launch_job (job *j, int foreground){
    process *p;
    pid_t pid;
    int mypipe[2], infile, outfile;

    infile = j->stdin;
    for (p = j->first_process; p; p = p->next)
    {
        /* Set up pipes, if necessary.  */
        if (p->next)
        {
            if (pipe (mypipe) < 0)
            {
                perror ("pipe");
                exit (1);
            }
            outfile = mypipe[1];
        }
        else
            outfile = j->stdout;

        /* Fork the child processes.  */
        pid = fork ();
        if (pid == 0)
        /* This is the child process.  */
            launch_process (p, j->pgid, infile,
                            outfile, j->stderr, foreground);
        else if (pid < 0)
        {
            /* The fork failed.  */
            perror ("fork");
            exit (1);
        }
        else
        {
            /* This is the parent process.  */
            p->pid = pid;
            if (shell_is_interactive)
            {
                if (!j->pgid)
                    j->pgid = pid;
                setpgid (pid, j->pgid);
            }
        }

        /* Clean up after pipes.  */
        if (infile != j->stdin)
            close (infile);
        if (outfile != j->stdout)
            close (outfile);
        infile = mypipe[0];
    }

    if(!foreground)format_job_info (j, "launched");

    if (!shell_is_interactive)
        wait_for_job (j);
    else if (foreground)
        put_job_in_foreground (j, 0);
    else
        put_job_in_background (j, 0);
}

int minishell::mark_process_status (pid_t pid, int status)
{
    job *j;
    process *p;

    if (pid > 0)
    {
        /* Update the record for the process.  */
        for (j = first_job; j; j = j->next)
            for (p = j->first_process; p; p = p->next)
                if (p->pid == pid)
                {
                    p->status = status;
                    if (WIFSTOPPED (status))
                        p->stopped = 1;
                    else
                    {
                        p->completed = 1;
                        if (WIFSIGNALED (status))
                            fprintf (stderr, "%d: Terminated by signal %d.\n",
                                     (int) pid, WTERMSIG (p->status));
                    }
                    return 0;
                }
        fprintf (stderr, "No child process %d.\n", pid);
        return -1;
    }
    else if (pid == 0 || errno == ECHILD)
    /* No processes ready to report.  */
        return -1;
    else {
        /* Other weird errors.  */
        perror ("waitpid");
        return -1;
    }
}

/* Check for processes that have status information available,
 without blocking.  */

void minishell::update_status (void)
{
    int status;
    pid_t pid;

    do
        pid = waitpid (WAIT_ANY, &status, WUNTRACED|WNOHANG);
    while (!mark_process_status (pid, status));
}

/* Check for processes that have status information available,
 blocking until all processes in the given job have reported.  */

void minishell::wait_for_job (job *j)
{
    int status;
    pid_t pid;
    wait_flag = 1;
    do
        pid = waitpid (WAIT_ANY, &status, WUNTRACED);
    while (!mark_process_status (pid, status)
           && !job_is_stopped (j)
           && !job_is_completed (j));
    /* Exited cleanly from foreground, no notificationƒ */
    if(WIFEXITED(status))j->notified=1;
    wait_flag = 0;
    do_job_notification();
}

/* Format information about job status for the user to look at.  */

void minishell::format_job_info (job *j, const char *status)
{
    fprintf (stderr, "[%ld] (%s): %s\n", (long)j->pgid, status, j->command);
}

/* Notify the user about stopped or terminated jobs.
 Delete terminated jobs from the active job list.  */

void minishell::do_job_notification (void)
{
    job *j, *jlast, *jnext;

    /* Update status information for child processes.  */
    update_status ();

    jlast = NULL;
    for (j = first_job; j; j = jnext)
    {
        jnext = j->next;

        /* If all processes have completed, tell the user the job has
         completed and delete it from the list of active jobs.  */
        if (job_is_completed (j)) {
            if(!j->notified)format_job_info (j, "completed");
            if (jlast)
                jlast->next = jnext;
            else
                first_job = jnext;
            free_job (j);
        }

        /* Notify the user about stopped jobs,
         marking them so that we won’t do this more than once.  */
        else if (job_is_stopped (j) && !j->notified) {
            format_job_info (j, "stopped");
            j->notified = 1;
            jlast = j;
        }

        /* Don’t say anything about jobs that are still running.  */
        else
            jlast = j;
    }
}

void minishell::free_job(job* j){
    if(j){
        /* Free all process first */
        process* p = j->first_process;
        process* tmp;
        while(p){
            /* Free argv */
            int i=0;
            while(p->argv[i]){
                free(p->argv[i]);
                ++i;
            }
            free(p->argv);
            tmp = p->next;
            free(p);
            p = tmp;
        }
        free(j->command);
        free(j);
    }
}

/* Find the active job with the indicated pgid.  */
job * minishell::find_job (pid_t pgid)
{
    job *j;

    for (j = first_job; j; j = j->next)
        if (j->pgid == pgid)
            return j;
    return NULL;
}

/* Put job j in the foreground.  If cont is nonzero,
 restore the saved terminal modes and send the process group a
 SIGCONT signal to wake it up before we block.  */

void minishell::put_job_in_foreground (job *j, int cont)
{
    /* Put the job into the foreground.  */
    tcsetpgrp (shell_terminal, j->pgid);

    /* Send the job a continue signal, if necessary.  */
    if (cont)
    {
        tcsetattr (shell_terminal, TCSADRAIN, &j->tmodes);
        if (kill (- j->pgid, SIGCONT) < 0)
            perror ("kill (SIGCONT)");
    }

    /* Wait for it to report.  */
    wait_for_job (j);

    /* Put the shell back in the foreground.  */
    tcsetpgrp (shell_terminal, shell_pgid);

    /* Restore the shell’s terminal modes.  */
    tcgetattr (shell_terminal, &j->tmodes);
    tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);
}

/* Put a job in the background.  If the cont argument is true, send
 the process group a SIGCONT signal to wake it up.  */

void minishell::put_job_in_background (job *j, int cont)
{
    /* Send the job a continue signal, if necessary.  */
    if (cont)
        if (kill (-j->pgid, SIGCONT) < 0)
            perror ("kill (SIGCONT)");
}

/* Malloc job for parser */
job* minishell::malloc_job(std::string& command, std::vector<std::string>& args, fd3& rds){

    job* j = (job*)malloc(sizeof(job));
    process* p = (process*)malloc(sizeof(process));

    if(j&&p){
        /* Put job into job list */
        job* jlast = first_job;
        while(jlast&&jlast->next){
            jlast = jlast->next;
        }
        if(jlast){
            jlast->next = j;
        }else{
            first_job = j;
        }

        /* Next job is Null */
        j->next = NULL;

        /* Set job command */
        j->command = (char*)malloc(sizeof(char)*(strlen(command.c_str())+1));
        strcpy(j->command, command.c_str());
        j->first_process = p;
        j->pgid = 0;
        j->notified = 0;

        /* Set pipeline */
        j->stdin = (rds._in<0)?0:rds._in;
        j->stdout = (rds._out<0)?1:rds._out;
        j->stderr = (rds._err<0)?2:rds._err;
        /* Initilize process */
        /* Set argv */
        char**argv = (char**)calloc(sizeof(char*),(args.size()+2));
        argv[0] = (char*)malloc(sizeof(char)*command.size());
        strcpy(argv[0],command.c_str());
        for(int i=0; i<args.size(); i++){
            argv[i+1] = (char*)malloc(sizeof(char)*args[i].size());
            strcpy(argv[i+1],args[i].c_str());
        }

        p->next = NULL;
        p->argv = argv;

    }else{
        perror("Malloc Error : Cannot allocate new job\n");
        free(j);
        free(p);
    }

    return j;
}

int minishell::try_launch_job(std::string& cmd_raw, std::vector<std::string>& args, fd3& rds, char background){
    if(rds.fail)return 0;
    rds._in = (rds._in<0)?0:rds._in;
    rds._out = (rds._out<0)?1:rds._out;
    rds._err = (rds._err<0)?2:rds._err;

    command* cmd=NULL;
    if(cmd_raw == "bg")cmd = new bg(*this);
    else if(cmd_raw == "cd")cmd = new cd(*this);
    else if(cmd_raw == "echo")cmd = new echo(*this);
    else if(cmd_raw == "exec")cmd = new command(*this);
    else if(cmd_raw == "exit"){puts("\n\n[ Shell EXITED ]\n");return -1;}
    else if(cmd_raw == "fg")cmd = new fg(*this);
    else if(cmd_raw == "set" )cmd = new set(*this);
    else if(cmd_raw == "shift")cmd = new command(*this);
    else if(cmd_raw == "test")cmd = new command(*this);
    else if(cmd_raw == "time")cmd = new command(*this);
    else if(cmd_raw == "unset")cmd = new unset(*this);
    else if(cmd_raw == "jobs")cmd = new jobs(*this);

    /* Is a builtin command */
    if(cmd){
        if(background){
            fprintf(stderr,"shell : Built-in command executed in background is not supported\n");
            return 0;
        }else{
            cmd->set_args(args);
            cmd->execute();
            delete cmd;
            return 0;
        }
    }
    char valid = 0;
    /* Check all search path */
    auto paths = this->get_paths();

    //search all search path
    for(auto path : paths){
        if(fs::exists(fs::path(path+"/"+cmd_raw))){
            cmd_raw=fs::path(path+"/"+cmd_raw).string();
            valid = true;
            break;
        }
    }
    /* Not find in path, check if it is executable*/
    if(!valid){
        if(fs::exists(fs::path(cmd_raw))){
            valid = true;
        }

        if(valid){
            auto p = fs::status(cmd_raw).permissions();
            bool perm_x =((p & fs::perms::owner_exec) != fs::perms::none)|| ((p & fs::perms::group_exec) != fs::perms::none)|| ((p & fs::perms::others_exec) != fs::perms::none);

            if(!perm_x){
                valid = 0;
            }
        }
    }
    if(valid){
        job* j = this->malloc_job(cmd_raw,args,rds);
        if(j){
            this->launch_job(j,!background);
        }
    }else{
        fprintf(stderr,"shell : %s is not a command neither an executable file\n",cmd_raw.c_str());
    }

    return 0;
}

