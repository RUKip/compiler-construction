%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "lex.yy.c"

bucket* globalTable;
bucket* localTable;
int isGlobal; //0 if it aint global else 1
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
		     { 
		      if (isGlobal){
			printf("isGlobal ");
			printf("%s\n", yytext);
			insertSymbol(globalTable, yytext, 266); //TODO has to be a valid type not always REAL 
		      }else{
			printf("isLocal ");
			printf("%s\n", yytext);
			insertSymbol(localTable, yytext, 266);
		      }
		     }
                   | identlist ',' IDENTIFIER			     
                   { 
		      if (isGlobal){
			printf("isGlobal ");
			printf("%s\n", yytext);
			insertSymbol(globalTable, yytext, 266); //TODO has to be a valid type not always REAL 
		      }else{
			printf("isLocal ");
			printf("%s\n", yytext);
			insertSymbol(localTable, yytext, 266);
		      }
		     }
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

subprogheading     : FUNCTION {printf("test\n"); isGlobal = 0; free(localTable); localTable = initSymbolTable();} IDENTIFIER arguments ':' standardtype ';' {printf("%p: z\n", lookupSymbol(globalTable, "z"));}
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
  isGlobal = 1;
  globalTable = initSymbolTable();
  yyparse();
  finalizeLexer();
  free(globalTable);
  free(localTable);
  return EXIT_SUCCESS;
}
