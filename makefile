CC = g++
FLAGS = -IOBJ -Ibuilt-in -Wno-unused-value -Wno-deprecated-register -std=c++17 -g
CFLAGS = -Wall -g

all: lex shell
	$(CC) $(FLAGS) -c driver.cc -o OBJ/driver.o
	$(CC) $(FLAGS) -c main.cpp -o OBJ/main.o
	$(CC) $(FLAGS) OBJ/*.o -o minishell

lex: dir
	yacc -d -Wno-yacc parser.y -o OBJ/y.tab.cc
	lex -o OBJ/lex.yy.cc scanner.l
	$(CC) $(FLAGS) -c OBJ/y.tab.cc -o OBJ/parser.o
	$(CC) $(FLAGS) -c OBJ/lex.yy.cc -o OBJ/scanner.o
shell:
	$(CC) $(FLAGS) -c built-in/minishell.cpp -o OBJ/minishell.o

dir:
	[ -d OBJ ] || mkdir OBJ
	[ -d bin ] || mkdir bin

clean:
	rm -rf ./OBJ
	rm -rf ./bin/minishell
	rm minishell