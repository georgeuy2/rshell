# rshell
CS100 Assignment 3
# Introduction
This program runs as a bash terminal. It functions with the same commands a standard bash terminal and uses perror() to give feedback on the execution of a command. It now is able to run test (with flags: -e, -f, and -d) as well as use parentheses when inputting commands to change precedence.
#Install Instructions
To install this project:
```
$ git clone https://github.com/georgeuy2/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/rshell
```
#Known Bugs/Limitations
* Tabs, control + character, and arrow keys are treated as characters and not auto-finish.
* cd .. does not work while stringed together with another command via a connector.
* The command "wait" produces an error.
* Using cd with a comment produces an error but not the same error as a standard shell.
* The || connector does not work with certain commands missing their arguments (i.e. "ls- " or "mk".
