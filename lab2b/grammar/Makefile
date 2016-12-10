all: lex.yy.c
	gcc lex.yy.c -lfl
lex.yy.c: Grammar.fl
	flex Grammar.fl
