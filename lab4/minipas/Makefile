all: main

main : parser
	gcc -o minipas minipas.tab.o -lfl

parser: minipas.y minipas.fl symbolTable.c strtab.c
	bison -dv minipas.y
	flex minipas.fl
	gcc -c minipas.tab.c
	gcc -c symbolTable.c
	gcc -c strtab.c

clean:
	rm -f *~
	rm -f *.o
	rm -f minipas.tab.*
	rm -f lex.yy.*
