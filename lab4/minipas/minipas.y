%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"

bucket* globalTable; //TODO might be empty
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
			insertSymbol(globalTable, insertStringTable(yytext), 266); //TODO has to be a valid type not always REAL 
		      }else{
			printf("isLocal ");
			printf("%s\n", yytext);
			insertSymbol(localTable, insertStringTable(yytext), 266);
		      }
		     }
                   | identlist ',' IDENTIFIER			     
                   { 
		      if (isGlobal){
			printf("isGlobal ");
			printf("%s\n", yytext);
			insertSymbol(globalTable, insertStringTable(yytext), 266); //TODO has to be a valid type not always REAL 
		      }else{
			printf("isLocal ");
			printf("%s\n", yytext);
			insertSymbol(localTable, insertStringTable(yytext), 266);
		      }
		     }
                   ;

declarations       : declarations VAR identlist ':' type ';' //TODO add type here
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

subprogheading     : FUNCTION {printf("new function start\n"); isGlobal = 0; free(localTable); localTable = initSymbolTable();} IDENTIFIER arguments ':' standardtype ';'// {printf("z: %p\n", lookupSymbol(globalTable, "z"));}
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
  showStringTable();
  
  //debug
    int i, sum = 0;
  for(i=0; i<97; i++){
    if(globalTable[i] == NULL){
      sum++;
    }else{
      printf("element %d key: %s\n",i, globalTable[i]->key);
      printf("element %d type: %d\n",i, globalTable[i]->type);
    }
  }
  printf("sum: %d\n", sum);
  
  
  freeStringTable();
  finalizeLexer();

  
  //more debug
  if(globalTable[0] == NULL){
    printf("isNull globalTable\n");
  }else{
    printf("first element: %s\n", globalTable[0]->key);
  }
  free(globalTable);
  free(localTable);
  
  
  return EXIT_SUCCESS;
}
