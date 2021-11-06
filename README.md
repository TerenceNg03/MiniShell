# MiniShell

Implementing a shell with basic function with **ONLY** POXSI system call. 

### Build Environment
---
* OS version : `OSX 12.0.1`
* C++ version : `c++17`
* Compiler : `Apple Clang`
* Bison : `Bison 3.2+` This is used to generate interpreter

Note : This should be working on any OS implementing POXSI yet not tested.
### Supported Features
---

#### 1. Environment Variable Set and Unset

Here is a list of default environment variables.
* `HOME` : The path to user's home directory
* `PWD`: Current work directory
* `SHELL` : System default shell
* `PATH` : Executable file search path. Default to `./bin:/bin:/usr/bin` where `.` will be replaced with launch path, this setting shuold be able to execute most system executable command such as ps, cat and etc.


#### 2. Built-in Command List
Built-in command is command that will executed by shell itself. Currently redirection for such command is disabled.

* `bg` : Move job to background and continue to execute. `bg 1` means move the first job to background. Note that indexing job with `%n` will cause syntax error.
* `cd` : Change work directory. `cd ~/.vim` will enter `~/.vim`.
* `echo` : Print a string or something. `echo $PWD` will print environment variable `PWD`.
* `exit` : Quit shell. This is equivalent to press`ctrl+D`.
* `fg` : Move job to foreground, similar to `bg`.
* `jobs` : List all jobs and each process under the job.
* `set`: Print all environment variables.
* `shift` : Reserved.
* `test` : Reserved.
* `time` : Reserved.
* `unmask` : Reserved.

#### 3. Job Control

This feature is enabled only when shell is interactive.
#### 3.1 Foreground and Background Process

By default each command is run as an individual process and will take control of terminal while shell is waiting for it to quit. However, by appending `&` to the end of command, shell will run the process as a background process. A background process will run without controlling terminal and is immune to terminal signals.

#### 3.2 Passing Signal to Foreground Process
Here is a list of signals that possibly be support. Note this is terminal depenent.
* `ctrl + C` which is `SIGINT`, will terminate process unless handled.
* `ctrl + D` which is actually not a signal, will write an `EOF` to `stdin`.
* `ctrl + Z` which is `SIGTSTP`, will suspend a process unless handled.
* `ctrl + Y` similar to `ctrl + Z`, but will be sent when process attempt to read from stdin.

Other signal may also be support by your terminal. Checkout [GNU's description](https://www.gnu.org/software/bash/manual/html_node/Signals.html).
	
#### 3.3 Some Examples

* `cmd1 arg1 ; cmd2 arg2 arg3` will execute `cmd1` and after it finished `cmd2` will be executed.
* `cmd1 arg1& cmd2 arg2 &`will execute `cmd1` and `cmd2` concurrently.
* `fg 1` will bring the first job to foreground if existed.
* `jobs` will print a list of all current jobs.

#### 4. Shell Expansion
Shell expands some string automatically. See [GNU's description](https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html)
* `$VAR` will be replaced with shell's corresponding variable's content.
* `~` will be replaced with `$HOME`
* `" '` will be removed if paired. Otherwise it will cause a syntax error.
* Other expansion and expansion in string is not supported concurrently.
#### 5. Pipeline and Redirection

This feature is currently bugged.

### To Do List

- [x] Implement job control
- [x] Support redirection grammer
- [ ] Fixing redirection bugs
- [ ] Support environment variable assignment
- [ ] Support pipeline between commands
- [ ] Support filename expansion
