export

CC = clang++
CFLAG = -std=c++17 -Wall -fno-exceptions
INCLUDE = -I$(shell pwd)/include
DEFINE = -Dregister
FLAGS = ${CFLAG} ${INCLUDE} ${DEFINE}

LD = ld
LFLAGS = -r

RM = -@rm -f

BIN = $(shell pwd)/bin

.PHONY:  release subdirs all debug run

release: all

debug: CFLAG += -g
debug: all

run: all
	@./minishell
all: main.o subdirs
	@mkdir -p ${BIN} 
	${CC} ${CFLAGS} main.o lex/lex.o shell/shell.o -o ${BIN}/minishell
	@ln -f bin/minishell ./minishell

subdirs:
	${MAKE} -C lex lex.o
	${MAKE} -C shell shell.o
	${MAKE} -C external all

main.o: main.cpp
	${CC} ${FLAGS} -c main.cpp

clean:
	${MAKE} -C lex clean
	${MAKE} -C shell clean
	${MAKE} -C external clean
	${RM} *.o
	${RM} bin/minishell
	${RM} minishell
	${RM} *.log
