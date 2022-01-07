# MiniShell

Implementing a shell with basic function with **ONLY** POSIX system call. 

### Build Environment
---
* OS version : `OSX 10.15.7+`
* C++ version : `c++17`
* Compiler : `Apple Clang 13.0.0`
* Bison : `GNU Bison 3.2+`
* flex : `flex 2.6.4`

### Supported Features
---

Note : This shell implement does not fully satisfy POSIX standard. See [POSIX shell standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html).
#### 1. Environment Variable Set and Unset

Here is a list of default environment variables.
* `HOME` : The path to user's home directory
* `PWD`: Current work directory
* `SHELL` : System default shell
* `PATH` : Executable file search path. Default to `./bin:/bin:/usr/bin` where `.` will be replaced with launch path, this setting should be able to execute most system executable command such as ps, cat and etc.


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
Here is a list of signals that possibly be support. Note this is terminal dependent.
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

#### 3.4 Concepts of Jobs and Process

This two concepts can often cause confusion. Basically, a job is a group of processes that share the same **pgid** or **process group id**. This is independent of **pid** and can be modified after process has been launched while pid can not.

A process can **control** a terminal if it has the same **pgid** as the terminal's. By controlling a terminal, a process is able to read input from it and receive signals that is sent to terminal's process group. In such condition, a process is called a **Foreground process**. Otherwise, it is a **Background process**. Both kinds of process is able to write output to terminal.

See [GNU : Implementing a Job Control Shell](https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html) for some guidance.
#### 4. Shell Expansion
Shell expands some string automatically. See [GNU's description](https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html)
* `$VAR` will be replaced with shell's corresponding variable's content.
* `~` will be replaced with `$HOME`.
* `\` is used as escape character. For instance, `"\n\006\\"`will print newline, ascii`\06`and a `\`. Outside string `\` is also used as escaped character. For example, `\>` will be interpreted as `>`.
* `"asdf" 'asdf'` String is quoted by `'""` and must be paired unless escaped by `\`. Otherwise it will cause a syntax error.
* Other expansion and expansion in string is not supported concurrently.
#### 5. Pipeline and Redirection

#### 5.1 Redirection

Shell support redirecting a job's input or output to **a specific file** or a **file descriptor** .

Here is a list of file descriptor :
* `0` for stdin 
* `1` for stdout
* `2` for stderr
* Other file descriptor is considered as invalid.

The following grammars are allowed :

* `command > filename` or `command >& file descriptor`
* `command 0> filename` or `command 0>& file descriptor` Note: There must be no space between `0` and `>`.
* `>`(write) can also be replaced by `>>`(append) or `<`(read).
* `command < ...` will redirect stdin.
* `command > ...` will redirect stdout. If it is redirected to a file, shell will create a new file or clear original file.
* `command >> ...` will redirect stdout and append output to file.
* `<<` operator is not supported though in POSIX standard.

#### 5.2 Pipeline

Shell can setup pipeline between different process. A process can read input from the output that the previous process generates.

The grammar is `command1 args1 | command2 args2 | ... | commandn argsn`

#### 6 ShowCase
<img width="1190" alt="Screen Shot 2021-11-16 at 10 25 59 PM" src="https://user-images.githubusercontent.com/63455223/142009310-2776fe3a-8508-447d-be17-041281925818.png">


### To Do List

- [x] Implement job control
- [x] Support redirection grammar
- [x] Support escape sequence
- [x] Fixing redirection bugs
- [ ] Support environment variable assignment
- [x] Support pipeline between commands
- [ ] Support filename expansion
