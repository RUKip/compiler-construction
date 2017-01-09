%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"

bucket* globalTable; 
bucket* localTable;
int isGlobal; //0 if it aint global else 1
int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

//TODO getType gives a segmentation fault
int getType(char *strtabEntry){ //Returns the type of an variable, does also check if it exists in stringtable

  printf("%s is our string\n", strtabEntry);

/*  if(lookupStringTable(strtabEntry) == NULL){
    printf("variable not delcared\n");
    exit(-1);
  }
  */
  
  int typeLocal = *((int *) lookupSymbol(localTable, strtabEntry));
  printf("debug typelocal: %d\n", typeLocal);
  printf("debug lookupSymbol: %d\n", lookupSymbol(localTable, strtabEntry));
  if(typeLocal == 0){ //linearSearch returns type and 0 if no type is found.
    if (*((int *) lookupSymbol(globalTable, strtabEntry)) == 0){
      printf("variable not declared or not accesible\n");
      exit(-1);
    }
    return *((int *) lookupSymbol(globalTable, strtabEntry));
  }
  return typeLocal; 
}

void checkDoubleDeclaration(char *strtabEntry){
    if (*((int *) lookupSymbol(localTable, strtabEntry)) == 0){
     if (*((int *) lookupSymbol(globalTable, strtabEntry)) == 0){
	return; //everything is fine not declared before
      } 
    }
    printf("already declared double declaration\n");
    exit(-1);
}

%}

//TODO should check: types at calculations/assignments, availablity and initialization of variables, number of arguments in funcion call, type of arguments in funtion call, double declarations(check global and local), function return type.

%union {
  int type;
  char *strtabptr;
}

%token PROGRAM IDENTIFIER VAR ARRAY RANGE NUMBER OF INTEGER REAL
       FUNCTION PROCEDURE BEGINTOK ENDTOK ASSIGN IF THEN ELSE WHILE DO
       RELOP MULOP

       
%type <type> NUMBER expression factor term simpleexpr
%type <strtabptr> IDENTIFIER
       
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

declarations       : declarations VAR identlist ':' type ';' //TODO add type here?
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
		      /*{
			if(!(getType($1) == $3)){
			  printf("type mismatch\n");
			  exit(-1);
			};
		      }*/
                   | procstatement
                   | compoundstatement
                   | IF boolexpression THEN statement ELSE statement
                   | WHILE boolexpression DO statement
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

boolexpression	   : simpleexpr RELOP simpleexpr
		   ;
                   
expression         : simpleexpr	
                   ;

simpleexpr         : term
                   | sign term			{$$ = $2;}
                   | simpleexpr '+' term 	{$$ = $1 + $3;}
                   | simpleexpr '-' term	{$$ = $1 - $3;}
                   ;

sign               : '+'
                   | '-'
                   ;

term               : factor
                   | term MULOP factor
                   ;

factor             : IDENTIFIER 			{$$ = getType($1);}
                   | IDENTIFIER '(' exprlist ')' 	{$$ = getType($1);} //TODO implement
                   | NUMBER				{$$ = $1;}
                   | '(' expression ')'			{$$ = $2;}
                   ;

%%

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pasfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  //newstuff probably remove and ofcourse underlying function
  initializeStringTable(argv[1]);
  //endnewstuff
  
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
