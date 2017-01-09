%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"

bucket* globalTable; 
bucket* localTable;
int isGlobal; //0 if it aint global else 1
int currentType = 0;
char** identifierList;

int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

//TODO getType gives a segmentation fault
int getType(char *strtabEntry){ //Returns the type of an variable, does also check if it exists in stringtable

  printf("%s is our string\n", strtabEntry);

 if(lookupStringTable(strtabEntry) == NULL){
    printf("variable not delcared\n");
    exit(-1);
  }
  int typeLocal = lookupSymbol(localTable, strtabEntry);
  printf("debug typelocal: %d\n", typeLocal);
  if(typeLocal == 0){ //linearSearch returns type and 0 if no type is found.
    if (lookupSymbol(globalTable, strtabEntry) == 0){
      printf("variable not declared or not accesible\n");
      exit(-1);
    }
    return lookupSymbol(globalTable, strtabEntry);
  }
  return typeLocal; 
}

void checkDoubleDeclaration(char *strtabEntry){
    if (lookupSymbol(localTable, strtabEntry) == 0){
     if (lookupSymbol(globalTable, strtabEntry) == 0){
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

       
%type <type> NUMBER expression factor term simpleexpr type standardtype
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
						
					}
                   | identlist ',' IDENTIFIER		     
                   { 
					}
                   ;

declarations       : declarations VAR  identlist ':' type ';' 
					{
						for(identifier:identifierList){
							  printf("typelocal: %d\n", currentType);
							  if (isGlobal){
								printf("isGlobal ");
								printf("%s\n", yytext);
								insertSymbol(globalTable, identifier, $5);
							  }else{
								printf("isLocal ");
								printf("%s\n", yytext);
								insertSymbol(localTable, identifier, $5);
							  }
						}
					} //TODO add type here?
	               | /* epsilon */
                   ;

type               : standardtype
                   | ARRAY '[' NUMBER RANGE NUMBER ']' OF standardtype {$$ = $8 + 2;} // 265 = int 267 = int[], 266 = real 268 = real[]
                   ;

standardtype       : INTEGER {$$ = 265;}
                   | REAL {$$ = 266;}
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
