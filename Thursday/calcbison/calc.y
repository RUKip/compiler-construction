/****** options section ********/
%{  /* verbatim copied in C file */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lex.yy.c"
#include "symtab.h"

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
%left '+' '-'
%left '*' '/'
%right '^'
%right NEG

%type <fval> Expr
%type <fval> INTCONST
%type <fval> FLOATCONST
%type <strtabptr> IDENTIFIER

%start Input

%%  /****** grammar rules section ********/

Input    : /* empty */
         | Input Line ';'
         ;

Line     : LET IDENTIFIER '=' Expr { insertSymbol($2, &($4), sizeof(float *)); }
         | Expr                    { printf("%.1f\n", $1); }
         ;
      
Expr     : INTCONST           
         | FLOATCONST          
         | IDENTIFIER          { $$ = * ((float*) lookupSymbol($1)); }
         | Expr '+' Expr       { $$ = $1 + $3; }
         | Expr '-' Expr       { $$ = $1 - $3; }
         | Expr '*' Expr       { $$ = $1 * $3; }
         | Expr '/' Expr       { $$ = $1 / $3; }
         | Expr '^' Expr       { $$ = powf($1, $3); }
         | '-' Expr %prec NEG  { $$ = -$2; }
         | '(' Expr ')'        { $$ = $2; }
         ;
         
%%   /****** main code section ********/

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <calcfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  initLexer(argv[1]);
  initSymbolTable();
  yyparse();
  freeSymbolTable();
  finalizeLexer();
  return EXIT_SUCCESS;
}

