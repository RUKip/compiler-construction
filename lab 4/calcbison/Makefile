CC=gcc
CFLAGS=-O2 -Wall -Wunused-function 
all: scanner parser strtab.o symtab.o
	${CC} -o calc ${CFLAGS} calc.tab.c strtab.o -ll -lm

scanner: calc.lex
	flex calc.lex
parser: calc.y
	bison -d -v calc.y
clean:
	rm -f calc.tab.c
	rm -f calc.tab.h
	rm -f lex.yy.c
	rm -f *.o
	rm -f *~
