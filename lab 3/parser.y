%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.c"

int yyerror(char *s) {
  showLine();
  printf("%s\n",s);
  exit(-1);
}
%}

%define parse.error verbose

%token ID NUM PROGRAM VAR ARRAY FUNCTION PROCEDURE
%token ASSIGNOP RELOP ADDOP MULOP PUNKTPUNKT
%token INTEGER REAL ANFANG ENDE OF IF THEN ELSE WHILE DO NOT
%%

program : PROGRAM ID '(' identifier_list ')' ';'
	declarations
	subprogram_declarations
	compound_statement
	'.'
	;
	
identifier_list : ID
	| identifier_list ',' ID
	;
	
declarations : declarations VAR identifier_list ':' type ';'
	| 
	;
	
type : standard_type
	| ARRAY '[' NUM PUNKTPUNKT  NUM ']' OF standard_type
	;
	
standard_type : INTEGER
	| REAL 
	;
	
subprogram_declarations : subprogram_declarations subprogram_declaration ';'
	|
	;
	
subprogram_declaration : subprogram_head declarations compound_statement
	;

subprogram_head : FUNCTION ID arguments ':' standard_type ';'
	| PROCEDURE ID arguments ';'
	;
	
arguments : '(' parameter_list ')'
	|
	;
	
parameter_list : identifier_list ':' type
	| parameter_list ';' identifier_list ':' type
	;
	
compound_statement : ANFANG optional_statements ENDE
	;
	
optional_statements : statement_list
	|
	;
	
statement_list : statement
	| statement_list ';' statement
	;
	
statement : variable ASSIGNOP expression
	| procedure_statement
	| compound_statement
	| IF expression THEN statement ELSE statement
	| WHILE expression DO statement
	;
	
variable : ID
	| ID '[' expression ']'
	;

procedure_statement : ID
	| ID '(' expression_list ')'
	;
	
expression_list : expression
	| expression_list ',' expression
	;
	
expression : simple_expression
	| simple_expression RELOP simple_expression
	;
	
simple_expression : term
	| sign term
	| simple_expression ADDOP term
	;	

term : factor
	| term MULOP factor
	;
	
factor : ID
	| ID '(' expression_list ')'
	| NUM
	| '(' expression ')'
	| NOT factor
	;
	
sign : '+'
	| '-'
	;
//probably useless	
%%

int main(int argc, char* argv[]){
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pascalFile>\n", argv[0]);
    return -1;
  }
  initLexer(argv[1]);
  yyparse();
  finalizeLexer();
  return 0;
}
