all: lex.yy.c
	gcc Grammar.c lex.yy.c -lfl
lex.yy.c: Parser.fl
	flex Parser.fl
