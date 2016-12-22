/****** options section ********/
%{  /* verbatim copied in C file */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lex.yy.c"

int yyerror(char *s) {
  showLine();
  printf("%s\n",s);
  exit(EXIT_FAILURE);
}

%}

/* Bison declarations.  */
%union {
  float fval;
  char *strtabptr;
}

%token FLOATCONST INTCONST BOOLCONST
%token IDENTIFIER VAR INT REAL
%token LET 

%start Input

%%  /****** grammar rules section ********/

Input    : /* empty */
         | Input Line ';'
         ;

Line     : LET IDENTIFIER '=' Expr
         | Expr 
         ;
      
Expr     : INTCONST             
         | FLOATCONST
         | IDENTIFIER
         | Expr '+' Expr       
         | Expr '-' Expr       
         | Expr '*' Expr       
         | Expr '/' Expr        
         | '-' Expr
         | '(' Expr ')'
         ;
         
%%   /****** main code section ********/

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <calcfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  initLexer(argv[1]);
  yyparse();
  showStringTable();
  freeStringTable();
  finalizeLexer();
  return EXIT_SUCCESS;
}

