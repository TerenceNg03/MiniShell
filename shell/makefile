.PHONY: clean

shell.o: minishell.o builtin.o job.o

	${LD} ${LFLAGS} minishell.o builtin.o job.o -o shell.o

minishell.o: minishell.cpp

	${CC} ${FLAGS} -c minishell.cpp

builtin.o: builtin.cpp
	
	${CC} ${FLAGS} -c builtin.cpp

job.o:

	${CC} ${FLAGS} -c job.cpp

clean:
	${RM} *.o
