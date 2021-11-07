.PHONY: clean

lex.o: scanner.o parser.o driver.o scanner.o
	
	${LD} ${LFLAGS} scanner.o parser.o driver.o -o lex.o
	

scanner.o: scanner.l scanner.hh parser.o

	lex -o lex.yy.cc scanner.l
	${CC} ${FLAGS} -c lex.yy.cc -o scanner.o

parser.o: parser.y

	${YACC} -d -Wno-yacc parser.y -o y.tab.cc
	${CC} ${FLAGS} -c y.tab.cc -o parser.o

driver.o: driver.cc scanner.o parser.o

	${CC} ${FLAGS} -c driver.cc -o driver.o

clean:
	${RM} location.hh
	${RM} position.hh
	${RM} *.o
	${RM} y.tab.*
	${RM} lex.yy.cc
	${RM} stack.hh
