# OSN Mini Project 1 (C shell)
Amey Choudhary

# Description
The following contains implementation of the [Mini Project 1](https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).

# List of Specifications:

### Display Requirement:
The shell prompt displays the username, system name, and current directory path. The format is: <Username@SystemName:~>

### Input Requirements
1. Supports ; and & separated list of commands.
2. Handles random spaces and tabs in input.
3. Commands separated by ; are executed sequentially.
4. Commands followed by & are executed in the background, printing the process ID.


### warp Command
Changes the current working directory and prints the new path.
Supports . (current directory), .. (parent directory), ~ (home directory), and - (previous directory) symbols.
Executes multiple arguments sequentially.
Example: <JohnDoe@SYS:~> warp test /home/johndoe/test


Specification 4: peek Command
Lists files and directories in lexicographic order.
Supports -a (all files, including hidden) and -l (detailed information) flags.
Color-codes output: green for executables, white for files, blue for directories.
Specification 5: pastevents Command
Stores the 15 most recent command statements.
pastevents purge clears all stored events.
pastevents execute <index> executes the command at the specified index.
Specification 6: System Commands
Executes standard system commands in foreground or background.
Prints the process name and duration if a foreground process takes more than 2 seconds.
Specification 7: proclore Command
Prints process information such as PID, status, process group, virtual memory, and executable path.
Specification 8: seek Command
Searches for files or directories in the specified directory.
Supports -d (directories only), -f (files only), and -e (print or change directory if only one match found) flags.
Specification 9: I/O Redirection
Supports >, >>, and < for output and input redirection.
Displays error if input file not found.
Specification 10: Pipes
Supports piping between commands.
Specification 11: Redirection with Pipes
Supports combining I/O redirection and pipes.
Specification 13: Signals
ping <pid> <signal_number> sends signals to processes.
Handles Ctrl-C, Ctrl-D, and Ctrl-Z for signal management.
Specification 14: fg and bg Commands
fg <pid> brings a background process to foreground.
bg <pid> changes a stopped background process to running.
Specification 15: neonate Command
Prints the PID of the most recently created process every specified seconds until 'x' is pressed.
Specification 16: iMan Command
Fetches man pages from the internet using sockets.

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
