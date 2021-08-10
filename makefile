CC = g++
FLAGS = -IOBJ -Wno-unused-value -Wno-deprecated-register -std=c++14 -g
CFLAGS = -Wall -g

all: lex
	$(CC) $(FLAGS) -c driver.cc -o OBJ/driver.o
	$(CC) $(FLAGS) -c main.cpp -o OBJ/main.o
	$(CC) $(FLAGS) OBJ/*.o -o bin/myshell

lex: dir
	yacc -d -Wno-yacc parser.yy -o OBJ/y.tab.cc
	lex -o OBJ/lex.yy.cc scanner.ll
	$(CC) $(FLAGS) -c OBJ/y.tab.cc -o OBJ/parser.o
	$(CC) $(FLAGS) -c OBJ/lex.yy.cc -o OBJ/scanner.o

dir:
	[ -d OBJ ] || mkdir OBJ
	[ -d bin ] || mkdir bin

clean:
	rm -rf ./OBJ
	rm -rf ./bin/myshell