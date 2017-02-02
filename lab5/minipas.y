%{
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"
#include "stack.h"
#include "outputToC.h"
#include "tempType.h"

bucket* globalTable; 
bucket* localTable;
int isGlobal; //0 if it aint global else 1
char* lastVariable; //cOutput

int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

//Note: to help see the difference with our old code and new ouput code we used the /*cOutput*/ comment behind new code

/* typelist:
 * int 			265
 * real 		266
 * int[]		267
 * real[]		268 
 * void			-1
 */

tempType getType(char *strtabEntry){ //Returns the type of an variable, does also check if it exists in the scope

  int typeLocal = lookupSymbol(localTable, strtabEntry);
  struct tempType t;
  if(typeLocal == 0){ //linearSearch returns type and 0 if no type is found.
    if (lookupSymbol(globalTable, strtabEntry) == 0){
      printf("%s not declared or not accesible\n", strtabEntry);
      exit(-1);
    }
    t.type = lookupSymbol(globalTable, strtabEntry);
    return t;
  }
  t.type = typeLocal;
  return t; 
}

void checkDoubleDeclaration(char *strtabEntry, bucket* table){ //does what it says
    if (lookupSymbol(table, strtabEntry) == 0){
	return; //everything is fine not declared before
    } 
    printf("already declared, double declaration %s \n", strtabEntry);
    exit(-1);
}

void checkEqual(tempType t1, tempType t2){ //check type function/assignment (real/int is accepted)
    int type1 = t1.type;
    int type2 = t2.type;
    if(type1 == type2){
      return;
    }
    if((265 == type2|| type2 == 266) && (265 == type1 || type1 == 266)){
      return;
    }
    printf("type mismatch of %d with type %d\n", type1, type2);
    exit(-1);
}

void insertSymbolsFromStack(tempType t) {
	bucket* currentTable;
	  if (isGlobal) {
		  currentTable = globalTable;
	  } else {
		currentTable = localTable;
	  }
	while(!isEmpty()) {
		char* ident = pop();
		outputDeclaration(t, ident);//cOuput
		checkDoubleDeclaration(ident, currentTable);
		insertSymbol(currentTable, ident, t, 0);
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

void addArg(tempType t) {
    int arg = t.type;
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

void insertSymbolsAndArguments(tempType t){
	  bucket* currentTable;
	  if (isGlobal) {
		currentTable = globalTable;
		 
	  } else {
		 currentTable = localTable;
	  }
	
	  while(!isEmpty()) {
		char* ident = pop();
		storeArgument(t, ident);//cOuput
		checkDoubleDeclaration(ident, currentTable);
		insertSymbol(currentTable, ident, t, 0);
		addArg(t);
	  }
	
	  freeStack();
}

%}

%union {
  struct tempType tempType; //cOutput
  char *strtabptr;
  int label; //cOutput
}

%token PROGRAM IDENTIFIER VAR ARRAY RANGE NUMBER OF INTEGER REAL
       FUNCTION PROCEDURE BEGINTOK ENDTOK ASSIGN IF THEN ELSE WHILE DO
       RELOP MULOP READLN WRITELN

       
%type <tempType> NUMBER expression factor term simpleexpr type standardtype INTEGER REAL boolexpression
%type <strtabptr> IDENTIFIER variable sign multop relationop ASSIGN subprogheading
%type <label> ifrule thenrule whilerule dorule
       
%%

program            : PROGRAM IDENTIFIER '(' identlist ')' ';' {freeStack();}
                     declarations
	                 subprogdecls	{outputMain();}//cOutput
	                 compoundstatement
	                 '.' {outputEndMain();}//cOutput
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
                   | ARRAY '[' NUMBER RANGE NUMBER ']' OF standardtype {struct tempType t = $8; t.type + 2; $$ = t;} 
                   ;

standardtype       : INTEGER 
                   | REAL
                   ;

subprogdecls       : subprogdecls subprogdecl ';'
                   | /* epsilon */
                   ;

subprogdecl        : subprogheading declarations compoundstatement 
			{
				outputString("return "); outputString($1); outputEnd(); 
				outputString("}\n");
			}//cOutput
                   ;

subprogheading     : FUNCTION {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments(); initStoredArguments();/*cOutput*/} IDENTIFIER arguments ':' standardtype ';' 
						{
							outputFunctionName($6, $3); //cOuput
							checkDoubleDeclaration($3, localTable); 
							insertSymbol(localTable, $3, $6, 0); 
							checkDoubleDeclaration($3, globalTable); 
							insertSymbol(globalTable, $3, $6, 1); 
							addArguments(globalTable,$3, arguments, argIndex); 
							free(arguments);
							$$ = $3;
						}
                   | PROCEDURE {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments(); initStoredArguments();/*cOutput*/} IDENTIFIER arguments ';' 
						{
							struct tempType t; t.type = -1; //cOuput
							outputFunctionName(t, $3); //cOuput
							checkDoubleDeclaration($3, globalTable); 
							insertSymbol(globalTable, $3, t, 1); 
							addArguments(globalTable,$3, arguments, argIndex); 
							free(arguments);
							$$ = "";
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

statement          : variable ASSIGN expression 
			{
				checkEqual(getType($1),$3);
				outputString($1); outputString(" = "); outputOldTemp($3.temp); outputEnd(); //cOuput
			}  
                   | procstatement
                   | compoundstatement
                   | ifrule expression thenrule 
			{
				outputString("if(!"); outputOldTemp($2.temp); outputString(")"); outputString("goto "); outputLabel($1); outputEnd(); //cOuput
			}
			statement 
			{
				outputString("goto "); outputLabel($3); outputEnd(); //cOuput
				outputLabel($1); outputString(": "); outputEnd(); //cOuput
			}
			ELSE statement 
			{
				outputLabel($3); outputString(": "); outputEnd(); //cOuput
			} 
                   | whilerule expression dorule 
			{
				outputLabel($1); outputString(": "); outputEnd(); //cOutput
				outputString("if(!"); outputOldTemp($2.temp); outputString(")"); outputString("goto "); outputLabel($3); outputEnd(); //cOutput
			} 
			statement 
			{
				outputString("goto "); outputLabel($1); outputEnd(); //cOutput
				outputLabel($3); outputString(": "); outputEnd(); //cOutput
			} 
                   ;
                   
ifrule		   : IF {$$ = getLabel();} //cOutput
		   ;
		   
thenrule	   : THEN {$$ = getLabel();} //cOutput
		   ;
		   
whilerule	   : WHILE {$$ = getLabel();} //cOutput
		   ;
		  
dorule		   : DO {$$ = getLabel();} //cOutput
		   ;

variable           : IDENTIFIER
                   | IDENTIFIER '[' expression ']'
                   ;

procstatement      : IDENTIFIER
                   | IDENTIFIER '(' exprlist ')'	{getType($1); if(!isFunction(globalTable, $1)) {printf("%s is not a function\n", $1); exit(-1);} checkArguments(globalTable, $1, arguments, argIndex); free(arguments);}
                   | WRITELN '(' exprlist ')' 		
							{	
								outputPrintf();
								free(arguments);
								//todo free varlist here
							}
                   | READLN '(' exprlist ')'		{
								outputScanf();
								free(arguments);
								//todo free varlist here
							}
                   ;

exprlist           : expression					{initArguments(); initVarList(); addArg($1); addVar(lastVariable); initTempList(); storeToTempList($1.type);/*cOutput*/}
                   | exprlist ',' expression			{addArg($3); addVar(lastVariable); storeToTempList($3.type);/*cOutput*/}
                   ;

boolexpression	   : simpleexpr relationop simpleexpr
						{
							checkEqual($1,$3); 
							outputTempValue($1); outputOldTemp($1.temp); outputString($2); outputOldTemp($3.temp); outputEnd(); //cOuput 
							struct tempType t = $1; t.temp = getLastTemp(); $$ = t; //cOuput
						}
				   ;
				   
relationop         : RELOP {char *tmp = malloc((strlen(yytext) + 1) * sizeof(char)); strcpy(tmp, yytext); $$ = tmp;} //to fix THEN/DO from cOutput
				   ;
                   
expression         : simpleexpr	
				   | boolexpression
                   ;

simpleexpr         : term
                   | sign term				{outputTempValue($2); outputString($1); outputLastTemp(); outputEnd(); struct tempType t = $2; t.temp = getLastTemp(); $$ = t;} //cOuput
                   | simpleexpr '+' term
						{
							checkEqual($1,$3); 
							outputTempValue($1); outputOldTemp($1.temp); outputString(" + "); outputOldTemp($3.temp); outputEnd(); //cOuput
							struct tempType t = $1; t.temp = getLastTemp(); $$ = t; //cOuput
						}
                   | simpleexpr '-' term	
						{
							checkEqual($1,$3); 
							outputTempValue($1); outputOldTemp($1.temp); outputString(" - "); outputOldTemp($3.temp); outputEnd(); //cOuput
							struct tempType t = $1; t.temp = getLastTemp(); $$ = t; //cOuput
						}
                   ;

sign               : '+' {$$ = "+";}
                   | '-' {$$ = "-";}
                   ;

term               : factor
                   | term multop factor 
						{
							checkEqual($1,$3); 
							outputTempValue($1); outputOldTemp($1.temp); outputMulop($2); outputOldTemp($3.temp); outputEnd(); //cOuput
							struct tempType t = $1; t.temp = getLastTemp(); $$ = t; //cOuput
						}
                   ;

multop		   : MULOP {$$=yytext;}   
		   ;
                   
factor             : IDENTIFIER 					{outputTempValue(getType($1)); outputString($1); outputEnd(); struct tempType t; t = getType($1); t.temp = getLastTemp(); $$ = t; lastVariable = $1;} //cOuput 
                   | IDENTIFIER '(' exprlist ')' 		
						{
							if(!isFunction(globalTable, $1)) {printf("%s is not a function\n", $1); exit(-1);}
							checkArguments(globalTable, $1, arguments, argIndex);
							free(arguments);
							outputTempValue(getType($1)); outputString($1); outputTempList(); outputEnd(); //cOutput
							struct tempType t; t = getType($1); t.temp = getLastTemp(); $$ = t; //cOuput
						} 
                   | NUMBER					{outputTempValue($1); outputString(yytext); outputEnd(); struct tempType t = $1; t.temp = getLastTemp(); $$ = t;} //cOuput 
                   | '(' expression ')'				{outputTempValue($2); outputLastTemp(); outputEnd(); struct tempType t = $2; t.temp = getLastTemp(); $$ = t;} //cOuput 
                   ;

%%

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pasfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  initializeOutput();
  initializeStringTable(argv[1]);
  initLexer(argv[1]);
  isGlobal = 1;
  globalTable = initSymbolTable();
  yyparse();  
  freeStringTable();
  finalizeLexer();
  freeSymbolTable(globalTable);
  freeSymbolTable(localTable);
  
  
  return EXIT_SUCCESS;
}
