export

CC = clang++
CFLAG = -std=c++17 -Wall
INCLUDE = -I$(shell pwd)/include
DEFINE = -Dregister
FLAGS = ${CFLAG} ${INCLUDE} ${DEFINE}

LD = ld
LFLAGS = -r

RM = -@rm -f

BIN = $(shell pwd)/bin

.PHONY:  all lex shell external clean

all: main.o lex shell external
	@mkdir -p ${BIN} 
	${CC} ${CFLAGS} main.o lex/lex.o shell/shell.o -o ${BIN}/minishell
	@ln -f bin/minishell ./minishell

lex:
	${MAKE} -C lex lex.o

shell:
	${MAKE} -C shell shell.o

external:
	${MAKE} -C external all

main.o: main.cpp
	${CC} ${FLAGS} -c main.cpp

clean:
	${MAKE} -C lex clean
	${MAKE} -C shell clean
	${MAKE} -C external clean
	${RM} *.o
	${RM} bin/* bin/*.*
	${RM} minishell
