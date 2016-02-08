COMPILE = g++
FLAGS = -W -Werror -pedantic -ansi

main: main.cpp
	$(COMPILE) $(FLAGS) main.cpp
