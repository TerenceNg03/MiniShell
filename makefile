export

CC = g++
CFLAG = -std=c++17 -Wall
INCLUDE = -I$(shell pwd)/include
DEFINE = -Dregister
FLAGS = ${CFLAG} ${INCLUDE} ${DEFINE}

LD = ld
LFLAGS = -r

RM = -@rm -f

all: 
	${MAKE} -C lex lex.o
	${MAKE} -C shell shell.o
	@mkdir -p bin/
	${CC} ${CFLAGS} main.o lex/lex.o shell/shell.o -o bin/minishell
	@ln -f bin/minishell ./minishell



main.o: main.cpp
	${CC} ${FLAGS} -c main.cpp

bin/: bin/
	mkdir bin


clean:
	${MAKE} -C lex clean
	${MAKE} -C shell clean
	${RM} *.o
	${RM} bin/* bin/*.*
	${RM} minishell
