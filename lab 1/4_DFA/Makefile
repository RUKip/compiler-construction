all: lex.yy.c
	gcc lex.yy.c -lfl
lex.yy.c: DFA.fl
	flex DFA.fl
