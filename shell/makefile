shell.o: minishell.o builtin.o

	${LD} ${LFLAGS} minishell.o builtin.o -o shell.o

minishell.o: minishell.cpp

	${CC} ${FLAGS} -c minishell.cpp

builtin.o: builtin.cpp
	
	${CC} ${FLAGS} -c builtin.cpp

clean:
	${RM} *.o
