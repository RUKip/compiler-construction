all: lex.yy.c parser.tab.c parser.tab.h
	gcc lex.yy.c parser.tab.c parser.tab.h -lfl
lex.yy.c: scanner.fl
	flex scanner.fl
parser.tab.c parser.tab.h: parser.y
	bison -d parser.y
