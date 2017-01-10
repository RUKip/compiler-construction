%{
#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.c"

int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

%}

%token PROGRAM IDENTIFIER VAR ARRAY RANGE NUMBER OF INTEGER REAL
       FUNCTION PROCEDURE BEGINTOK ENDTOK ASSIGN IF THEN ELSE WHILE DO
       RELOP MULOP NOT

%%

program            : PROGRAM IDENTIFIER '(' identlist ')' ';'
                     declarations
	                 subprogdecls
	                 compoundstatement
	                 '.'
                   ;

identlist          : IDENTIFIER
                   | identlist ',' IDENTIFIER
                   ;

declarations       : declarations VAR identlist ':' type ';'
	               | /* epsilon */
                   ;

type               : standardtype
                   | ARRAY '[' NUMBER RANGE NUMBER ']' OF standardtype
                   ;

standardtype       : INTEGER
                   | REAL
                   ;

subprogdecls       : subprogdecls subprogdecl ';'
                   | /* epsilon */
                   ;

subprogdecl        : subprogheading declarations compoundstatement
                   ;

subprogheading     : FUNCTION IDENTIFIER arguments ':' standardtype ';'
                   | PROCEDURE IDENTIFIER arguments ';'
                   ;

arguments          : '(' paramlist ')'
                   | /* epsilon */
                   ;

paramlist          : identlist ':' type
                   | paramlist ';' identlist ':' type
                   ;

compoundstatement  : BEGINTOK optionalstatements ENDTOK
                   ;

optionalstatements : statementlist
                   | /* epsilon */
                   ;

statementlist      : statement
                   | statementlist ';' statement
                   ;

statement          : variable ASSIGN expression
                   | procstatement
                   | compoundstatement
                   | IF expression THEN statement ELSE statement
                   | WHILE expression DO statement
                   ;

variable           : IDENTIFIER
                   | IDENTIFIER '[' expression ']'
                   ;

procstatement      : IDENTIFIER
                   | IDENTIFIER '(' exprlist ')'
                   ;

exprlist           : expression
                   | exprlist ',' expression
                   ;

expression         : simpleexpr
                   | simpleexpr RELOP simpleexpr
                   ;

simpleexpr         : term
                   | sign term
                   | simpleexpr '+' term
                   | simpleexpr '-' term
                   ;

sign               : '+'
                   | '-'
                   ;

term               : factor
                   | term MULOP factor
                   ;

factor             : IDENTIFIER
                   | IDENTIFIER '(' exprlist ')'
                   | NUMBER
                   | '(' expression ')'
                   | NOT factor
                   ;

%%

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pasfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  initLexer(argv[1]);
  yyparse();
  finalizeLexer();
  return EXIT_SUCCESS;
}
