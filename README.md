# OSN Mini Project 1 (C shell)
Amey Choudhary

# Description
The following contains implementation of the [Mini Project 1](https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1).

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
