COMPILE = g++ 
FLAGS = -W -Werror -pedantic -ansi

all: rshel

rshell:
	mkdir -p bin
	$(COMPILE) $(FLAGS) src/main.cpp -o bin/rshell
