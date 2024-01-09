[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# OSN Mini Project 1
Amey Choudhary

2021113017

# Description
The following contains the part A of the Mini Project 1.

# Disclaimer
I have coded the entire codebase and used AI tools (ChatGPT and GitHub Copilot for assistance) for assistance in writing the code. I have tried to include their usage (if used) in comments.

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