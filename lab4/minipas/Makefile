all: main

main : parser
	gcc -o minipas minipas.tab.o -lfl

parser: minipas.y minipas.fl symbolTable.c
	bison -dv minipas.y
	flex minipas.fl
	gcc -c minipas.tab.c
	gcc -c symbolTable.c

clean:
	rm -f *~
	rm -f *.o
	rm -f minipas.tab.*
	rm -f lex.yy.*
