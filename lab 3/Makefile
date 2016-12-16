CC=gcc
CFLAGS=-O2 -Wall -Wunused-function 
all: scanner parser
	${CC} -o mpascal ${CFLAGS} parser.tab.c parser.tab.h -ll -lm

scanner: scanner.fl
	flex scanner.fl
parser: parser.y
	bison -d -v parser.y
clean:
	rm -f parser.tab.c
	rm -f parser.tab.h
	rm -f lex.yy.c
	rm -f *.o
	rm -f *~
