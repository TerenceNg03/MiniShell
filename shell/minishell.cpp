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
#include <stdio.h>


namespace fs = std::filesystem;
using std::string;
using std::vector;
using std::regex;
using std::optional;
using std::make_shared;
using std::shared_ptr;

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
#ifndef DEBUG
#define GrabTerminalControl 1
		/* Loop until we are in the foreground. */
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
#endif
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

void minishell::launch_process (process& p, pid_t pgid,
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
	char** argv;
	argv = (char**)calloc(sizeof(char*),(p.args.size()+1));
	if(!argv)perror("malloc :");

	for(int i=0; i<p.args.size(); i++){
		argv[i] = (char*)malloc(sizeof(char)*p.args[i].size());
		if(!argv[i])perror("malloc :");
		strcpy(argv[i],p.args[i].c_str());
	}

	execvp (argv[0], argv);
	perror ("execvp");
	exit (1);
}

void minishell::launch_job (job& j){
	pid_t pid;
	int mypipe[2], infile, outfile;
    bool forked = false;

	infile = j.stdin;
	for (auto p=j.processes.begin(); p!=j.processes.end(); p++)
	{
		if(p->is_builtin&&j.foreground){
			p->cmd->execute();
            p->completed = 1;
            j.notified = 1;
		}else{
			/* Set up pipes, if necessary.  */
			if (p!=prev(j.processes.end()))
			{
				if (pipe (mypipe) < 0)
				{
					perror ("pipe");
					exit (1);
				}
				outfile = mypipe[1];
			}
			else
				outfile = j.stdout;

			/* Fork the child processes.  */
			pid = fork ();
            forked = true;
			if (pid == 0){
				/* This is the child process.  */
				if(p->is_builtin){
					exit(p->cmd->execute());
				}else{
					launch_process (*p, j.pgid, infile, outfile, j.stderr, j.foreground);
				}
			}
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
					if (!j.pgid)
						j.pgid = pid;
					setpgid (pid, j.pgid);
				}
			}

            /* Clean up after pipes.  */
            if (infile != j.stdin)
                close (infile);
            if (outfile != j.stdout)
                close (outfile);
            infile = mypipe[0];

		}
	}

    /* Update job only when really forked */
    if(forked){

        if(!j.foreground)format_job_info (j, "launched");

        if (!shell_is_interactive)
            wait_for_job (j);
        else if (j.foreground)
            put_job_in_foreground (j, 0);
        else
            put_job_in_background (j, 0);
    }else{
        do_job_notification();
    }

}

int minishell::mark_process_status (pid_t pid, int status)
{
	if (pid > 0)
	{
		/* Update the record for the process.  */
        for (auto j = jobs.begin(); j!=jobs.end(); j =next(j))
            for (auto p = j->processes.begin(); p!= j->processes.end(); p = next(p))
                if (p->pid == pid)
				{
                    p->status = status;
                    if (WIFSTOPPED (status)){
                        p->stopped = true;
                    }
					else
					{
                        p->completed = true;
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

void minishell::wait_for_job (job& j)
{
	int status;
	pid_t pid;
	wait_flag = 1;
	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED);
	while (!mark_process_status (pid, status)
			&& !j.is_stopped()
			&& !j.is_completed());
	/* Exited cleanly from foreground, no notificationƒ */
	if(WIFEXITED(status))j.notified=1;
	wait_flag = 0;

	do_job_notification();
}

/* Format information about job status for the user to look at.  */

void minishell::format_job_info (job& j, const char *status)
{
	fprintf (stderr, "[%ld] (%s): %s\n", (long)j.pgid, status, j.command.c_str());
}

/* Notify the user about stopped or terminated jobs.
   Delete terminated jobs from the active job list.  */

void minishell::do_job_notification (void)
{

	/* Update status information for child processes.  */
	update_status ();

    auto jnext = jobs.begin();
	for (auto j=jobs.begin(); j!=jobs.end(); j = jnext)
	{
        jnext = next(j);
		/* If all processes have completed, tell the user the job has
		   completed and delete it from the list of active jobs.  */
		if (j->is_completed()) {
			if(!j->notified)format_job_info (*j, "completed");
			jobs.erase(j);
		}

		/* Notify the user about stopped jobs,
		   marking them so that we won’t do this more than once.  */
		else if (j->is_stopped() && !j->notified) {
			format_job_info (*j, "stopped");
			j->notified = 1;
		}

		/* Don’t say anything about jobs that are still running.  */
	}


}

/* Put job j in the foreground.  If cont is nonzero,
   restore the saved terminal modes and send the process group a
   SIGCONT signal to wake it up before we block.  */

void minishell::put_job_in_foreground (job& j, int cont)
{
	/* Put the job into the foreground.  */
	tcsetpgrp (shell_terminal, j.pgid);

	/* Send the job a continue signal, if necessary.  */
	if (cont)
	{
		tcsetattr (shell_terminal, TCSADRAIN, &j.tmodes);
		if (kill (- j.pgid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
	}

	/* Wait for it to report.  */
	wait_for_job (j);

	/* Put the shell back in the foreground.  */
	tcsetpgrp (shell_terminal, shell_pgid);

	/* Restore the shell’s terminal modes.  */
	tcgetattr (shell_terminal, &j.tmodes);
	tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);
}

/* Put a job in the background.  If the cont argument is true, send
   the process group a SIGCONT signal to wake it up.  */

void minishell::put_job_in_background (job& j, int cont)
{
	/* Send the job a continue signal, if necessary.  */
	if (cont)
		if (kill (-j.pgid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
}

/* Malloc process */
optional<process> minishell::resolve_command(std::string& cmd_raw, std::vector<std::string>& args){

	using namespace shellcmd;
	shared_ptr<command> cmd=NULL;
	if(cmd_raw == "bg")
		cmd = make_shared<bg>(bg(*this));
	else if(cmd_raw == "cd")
		cmd = make_shared<cd>(cd(*this));
	else if(cmd_raw == "exec")
		cmd = make_shared<command>(command(*this));
	else if(cmd_raw == "exit")
		cmd = make_shared<shellcmd::exit>(shellcmd::exit(*this));
	else if(cmd_raw == "fg")
		cmd = make_shared<fg>(fg(*this));
	else if(cmd_raw == "set" )
		cmd = make_shared<set>(set(*this));
	else if(cmd_raw == "shift")
		cmd = make_shared<command>(command(*this));
	else if(cmd_raw == "test")
		cmd = make_shared<command>(command(*this));
	else if(cmd_raw == "time")
		cmd = make_shared<command>(command(*this));
	else if(cmd_raw == "unset")
		cmd = make_shared<unset>(unset(*this));
	else if(cmd_raw == "jobs")
		cmd = make_shared<shellcmd::jobs>(shellcmd::jobs(*this));

	/* Is a builtin command */
	if(cmd){
		cmd->set_args(args);
		process p(cmd);
		return p;
	}

	char valid = 0;
	/* Check all search path */
	auto paths = this->get_paths();

	string cmd_cooked = cmd_raw;
	//search all search path
	for(auto path : paths){
		if(fs::exists(fs::path(path+"/"+cmd_cooked))){
			cmd_cooked=fs::path(path+"/"+cmd_cooked).string();
			valid = true;
			break;
		}
	}
	/* Not find in path, check if it is executable*/
	if(!valid){
		if(fs::exists(fs::path(cmd_cooked))){
			valid = true;
		}

		if(valid){
			auto p = fs::status(cmd_cooked).permissions();
			bool perm_x =((p & fs::perms::owner_exec) != fs::perms::none)|| ((p & fs::perms::group_exec) != fs::perms::none)|| ((p & fs::perms::others_exec) != fs::perms::none);

			if(!perm_x){
				valid = 0;
			}
		}
	}
	if(valid){
		process p(cmd_cooked,args);
		return p;
	}else{
		fprintf(stderr,"shell : %s is not a command neither an executable file\n",cmd_raw.c_str());
		return {};
	}

}

string minishell::resolve_str(string& s){
	string ret = "";
	s = s.substr(1,s.length()-2);
	int len = (int)s.length();
	bool translate=false;
	int num=0;
	for(int i=0; i<len; i++){
		if(translate){
			switch (s[i]) {
				case 'n':
					ret += '\n';
					break;
				case 'r':
					ret += '\r';
					break;
				case 't':
					ret += '\t';
					break;
				case 'a':
					ret += '\a';
					break;
				case 'f':
					ret += '\f';
					break;
				case 'v':
					ret += '\v';
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					num = 0;
					num += (s[i]-'0');
					if(i+1<len && s[i+1]<'8' && s[i+1]>='0'){
						num*=8;
						num+=s[i+1]-'0';
						i++;
					}
					if(i+1<len && s[i+1]<'8' && s[i+1]>='0'){
						num*=8;
						num+=s[i+1]-'0';
						i++;
					}
					ret += num;
					i++;
				default:
					ret += s[i];
					break;
			}
		}else{
			if(s[i]=='\\'){
				translate = true;
			}else{
				ret += s[i];
			}
		}
	}
	return ret;
}

string minishell::resolve_id(string& s){
	string ret = "";
	int len = (int)s.length();
	for(int i=0; i<len; i++){
		if(s[i]=='\\'){
			if(i+1<len){
				i++;
				ret+=s[i];
			}
		}else{
			ret += s[i];
		}
	}
	return ret;
}
