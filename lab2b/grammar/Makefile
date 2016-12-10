all: lex.yy.c
	gcc lex.yy.c -lfl
lex.yy.c: Parser.fl
	flex Parser.fl
