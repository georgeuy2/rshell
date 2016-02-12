# rshell
CS100 Assignment 2
# Introduction 
This program runs as a bash terminal. It functions with the same commands as a standard bash terminal and uses perror() to give feedback on the execution of command.
#Install Instructions
To install this project:
```
$ git clone https://github.com/georgeuy2/rshell.git
$ cd rshell
$ git checkout exec
$ make
$ bin/rshell
```
#Known Bugs/Limitations
* Tabs, control + character, and arrow keys are treated as characters and not auto-finish.
* cd .. does not work while stringed together with another command via a connector.
* Using cd with a comment produces an error but not the same error as a standard shell
* The || connector does not work with certain commands missing their arguments (i.e. "ls-" or "mkdir")  
