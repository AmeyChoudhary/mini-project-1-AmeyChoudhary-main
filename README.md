# OSN Mini Project 1 (C shell)
Amey Choudhary

# Description
The following contains implementation of the [Mini Project 1](https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).

# List of Specifications:
## Shell  Specifications

This document outlines the specifications for a shell program. The shell should meet the following requirements:

**User Interface**

* Display a prompt indicating the current working directory and username.
* Parse user input for commands and arguments.
* Handle `;` and `&` separated lists of commands.
* Support random spaces and tabs in user input.

**Built-in Commands**

* `warp` - change directory
    * Support absolute and relative paths, paths from the home directory, and multiple arguments.
    * Change the prompt to reflect the new working directory.
* `peek` - list files and directories
    * Support `-a` (all files), `-l` (long listing), and `.` (current directory), `..` (parent directory), `~` (home directory), and `-` (previous directory) arguments.
    * Display files in white, directories in blue, and executables in green.
* `pastevents` - manage command history
    * Store and display the 15 most recent commands.
    * Ignore commands containing `pastevents`.
    * Allow purging the history.
    * Execute commands from the history.
* `proclore` - display process information
    * Show process ID, status, group, virtual memory, and executable path.
* `seek` - search for files and directories
    * Support `-d` (directories only), `-f` (files only), and `-e` (change directory or print content) flags.
    * Handle flags and arguments.

**External Commands**

* Execute system commands like `emacs` and `gedit` in foreground and background processes.
* Print execution time for foreground processes taking more than 2 seconds.
* Display background process PIDs and termination messages.

**I/O Redirection**

* Support `>` (overwrite), `>>` (append), and `<` (read from file) redirection operators.
* Handle non-existent input files and create output files with appropriate permissions.

**Pipes**

* Connect multiple commands using pipes (`|`) to pass data between them.
* Support any number of pipes.

**Redirection with Pipes**

* Combine I/O redirection with pipes.

**Process Management**

* List running foreground and background processes spawned by the shell.
* Send signals to processes using `ping <pid> <signal_number>`.
* Handle keyboard interrupts (Ctrl-C), logout (Ctrl-D), and backgrounding (Ctrl-Z).
* Bring background processes to the foreground (`fg <pid>`) and vice versa (`bg <pid>`).

**Other Features**

* `neonate` - print the process ID of the most recently created process every n seconds until the user presses `x`.

**Networking**

* `iMan` - fetch man pages from a website and display them on the terminal.

**General Requirements**

* The shell should not use `execvp` or similar commands for specific functionalities.
* Color coding should be used to differentiate between files, directories, and executables in output.


# Steps to run the shell
1. Clone the repository into your local system.
2. Input "make" into the repository terminal.
3. It will create an executable file "a.out".Run it by "./a.out" in the terminal.

# Assumptions
1. The shell will run and display the username and hostname. It will also display the current working directory.
2. It is assumed that the input may be delimited by ";" and "&", but the arguments don't contain these delimiters.
3. The output of background processes will be printed to the terminal after they have been executed and their output is redirected to an external file.
4. The command "exit" will exit the shell. If any other argument is given along with it, it will not exit the shell.
5. If no correct path exists, the shell will remain inside the same directory.
6. Pastevents execute will only give the output for valid indexes.
7. Proclore will return an error on incorrect pid.
8. Seek will return no match found on no mattches or incorrect arguments
9. Peek returns error on incorrect directory.
10. Pastevents will also add those events to the history which were incorrect and also those run in background.
11. If empty, peek returns nothing.
