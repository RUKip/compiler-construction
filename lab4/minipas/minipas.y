%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"
#include "stack.h"

bucket* globalTable; 
bucket* localTable;
int isGlobal; //0 if it aint global else 1

int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

/* typelist:
 * int 			265
 * real 		266
 * int[]		267
 * real[]		268 
 * void			-1
 */

int getType(char *strtabEntry){ //Returns the type of an variable, does also check if it exists in the scope

  int typeLocal = lookupSymbol(localTable, strtabEntry);
  if(typeLocal == 0){ //linearSearch returns type and 0 if no type is found.
    if (lookupSymbol(globalTable, strtabEntry) == 0){
      printf("%s not declared or not accesible\n", strtabEntry);
      exit(-1);
    }
    return lookupSymbol(globalTable, strtabEntry);
  }
  return typeLocal; 
}

void checkDoubleDeclaration(char *strtabEntry, bucket* table){ //does what it says
    if (lookupSymbol(table, strtabEntry) == 0){
	return; //everything is fine not declared before
    } 
    printf("already declared, double declaration %s \n", strtabEntry);
    exit(-1);
}

void checkEqual(int type1, int type2){ //check type function/assignment (real/int is accepted)
    if(type1 == type2){
      return;
    }
    if((265 == type2|| type2 == 266) && (265 == type1 || type1 == 266)){
      return;
    }
    printf("type mismatch of %d with type %d\n", type1, type2);
    exit(-1);
}

void insertSymbolsFromStack(int type) {
	  if (isGlobal) {
		  while(!isEmpty()) {
			char* ident = pop();
			checkDoubleDeclaration(ident, globalTable);
			insertSymbol(globalTable, ident, type, 0);
		  }
	  } else {
		  while(!isEmpty()) {
			char* ident = pop();
			checkDoubleDeclaration(ident, localTable);
			insertSymbol(localTable, ident, type, 0);
		  }
	  }
	  freeStack();
}


int* arguments;
int argSize, argIndex;

void initArguments(){
	argSize=10;
	argIndex=0;
	arguments = malloc(argSize*sizeof(int)); 
}

void addArg(int arg) {
    if((argSize-1) == argIndex){
        int* temp = malloc(argSize*2*sizeof(int));    
        copy(temp, arguments);
        argSize *= 2;
        free(arguments);
        arguments = temp;    
    } 
    arguments[argIndex] = arg;
    argIndex++;
}

void insertSymbolsAndArguments(int type){
		  if (isGlobal) {
		  while(!isEmpty()) {
			char* ident = pop();
			checkDoubleDeclaration(ident, globalTable);
			insertSymbol(globalTable, ident, type, 0);
			addArg(type);
		  }
	  } else {
		  while(!isEmpty()) {
			char* ident = pop();
			checkDoubleDeclaration(ident, localTable);
			insertSymbol(localTable, ident, type, 0);
			addArg(type);
		  }
	  }
	  freeStack();
}

%}

//TODO should check: number of arguments in funcion call, type of arguments in funtion call, unused variable, not a void value in the readln/writeln arguments.

%union {
  int type;
  char *strtabptr;
}

%token PROGRAM IDENTIFIER VAR ARRAY RANGE NUMBER OF INTEGER REAL
       FUNCTION PROCEDURE BEGINTOK ENDTOK ASSIGN IF THEN ELSE WHILE DO
       RELOP MULOP READLN WRITELN

       
%type <type> NUMBER expression factor term simpleexpr type standardtype INTEGER REAL boolexpression
%type <strtabptr> IDENTIFIER variable
       
%%

program            : PROGRAM IDENTIFIER '(' identlist ')' ';'
                     declarations
	                 subprogdecls
	                 compoundstatement
	                 '.'
                   ;

identlist          : IDENTIFIER	
					{ 
						initStack();
						push($1);
					}
                   | identlist ',' IDENTIFIER		     
					{
					   push($3);
					}
                   ;

declarations       : declarations VAR  identlist ':' type ';' 
					{
						insertSymbolsFromStack($5);
					}
	               | /* epsilon */
                   ;

type               : standardtype
                   | ARRAY '[' NUMBER RANGE NUMBER ']' OF standardtype {$$ = $8 + 2;} // 265 = int 267 = int[], 266 = real 268 = real[]
                   ;

standardtype       : INTEGER 
                   | REAL
                   ;

subprogdecls       : subprogdecls subprogdecl ';'
                   | /* epsilon */
                   ;

subprogdecl        : subprogheading declarations compoundstatement
                   ;

subprogheading     : FUNCTION {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments();} IDENTIFIER arguments ':' standardtype ';' 
						{
							checkDoubleDeclaration($3, localTable); 
							insertSymbol(localTable, $3, $6, 0); 
							checkDoubleDeclaration($3, globalTable); 
							insertSymbol(globalTable, $3, $6, 1); 
							addArguments(globalTable,$3, arguments, argIndex); 
							free(arguments);
						}
                   | PROCEDURE {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments();} IDENTIFIER arguments ';' 
						{
							checkDoubleDeclaration($3, globalTable); 
							insertSymbol(globalTable, $3, -1, 1); 
							addArguments(globalTable,$3, arguments, argIndex); 
							free(arguments);
						}
                   ;

arguments          : '(' paramlist ')'
                   | /* epsilon */
                   ;

paramlist          : identlist ':' type
					{insertSymbolsAndArguments($3);}
                   | paramlist ';' identlist ':' type
					{insertSymbolsAndArguments($5);}
                   ;

compoundstatement  : BEGINTOK optionalstatements ENDTOK
                   ;

optionalstatements : statementlist
                   | /* epsilon */
                   ;

statementlist      : statement
                   | statementlist ';' statement
                   ;

statement          : variable ASSIGN expression {checkEqual(getType($1),$3);} 
                   | procstatement
                   | compoundstatement
                   | IF boolexpression THEN statement ELSE statement
                   | WHILE boolexpression DO statement
                   ;

variable           : IDENTIFIER
                   | IDENTIFIER '[' expression ']'
                   ;

procstatement      : IDENTIFIER
                   | IDENTIFIER '(' exprlist ')'	{getType($1); if(!isFunction(globalTable, $1)) {printf("%s is not a function\n", $1); exit(-1);} checkArguments(globalTable, $1, arguments, argIndex); free(arguments);}
                   | WRITELN '(' exprlist ')' 		{free(arguments);} //nothing has to happen except writln
                   | READLN '(' exprlist ')'		{free(arguments);} //nothing has to happen except readln
                   ;

exprlist           : expression					{initArguments(); addArg($1);}
                   | exprlist ',' expression	{addArg($3);}
                   ;

boolexpression	   : simpleexpr RELOP simpleexpr {checkEqual($1,$3);}
				   ;
                   
expression         : simpleexpr	
		           | boolexpression
                   ;

simpleexpr         : term
                   | sign term				{$$ = $2;}
                   | simpleexpr '+' term 	{$$ = $1; checkEqual($1,$3);}
                   | simpleexpr '-' term	{$$ = $1; checkEqual($1,$3);}
                   ;

sign               : '+'
                   | '-'
                   ;

term               : factor
                   | term MULOP factor {$$ = $1; checkEqual($1,$3);}
                   ;

factor             : IDENTIFIER 					{$$ = getType($1);}
                   | IDENTIFIER '(' exprlist ')' 	{$$=getType($1); if(!isFunction(globalTable, $1)) {printf("%s is not a function\n", $1); exit(-1);} checkArguments(globalTable, $1, arguments, argIndex); free(arguments);}
                   | NUMBER							{$$ = $1;}
                   | '(' expression ')'				{$$ = $2;}
                   ;

%%

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pasfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  initializeStringTable(argv[1]);
  initLexer(argv[1]);
  isGlobal = 1;
  globalTable = initSymbolTable();
  yyparse();

  freeStringTable();
  finalizeLexer();
  free(globalTable);
  free(localTable);
  
  
  return EXIT_SUCCESS;
}
