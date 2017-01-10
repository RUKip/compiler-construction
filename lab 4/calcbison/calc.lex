%{
#include <stdio.h>
#include <stdlib.h>
#include "calc.tab.h"
#include "strtab.h"

static char *inputbuffer;
int linenr = 1; 
%}

white       [ \t]+
identifier  [a-z]([a-z]|[0-9])*
digit       [0-9]
integer     {digit}+
exponent    [eE][+-]?{integer}

real        {integer}("."{integer})?{exponent}?

%%

{white}      { /* We ignore white characters */ }
\n           { linenr++; }
{integer}    { yylval.fval = atoi(yytext); return INTCONST; }
{real}       { yylval.fval = atof(yytext); return(FLOATCONST); }
let          { return LET; }
var          { return VAR; }
int          { return INT; }
real         { return REAL; }
{identifier} { yylval.strtabptr=insertStringTable(yytext); return IDENTIFIER; }
.            { return *yytext;  }

%%
void initLexer(char *filename) {
  FILE *f = fopen(filename, "r");
  int length;
  
  if (f == NULL) {
    fprintf(stderr, "Error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 0, SEEK_SET);
  inputbuffer = malloc (length+1);
  inputbuffer[length]='\0';
  length = fread (inputbuffer, 1, length, f);
  fclose (f);
  yyin = fopen(filename, "r");
}

void showLine() {
  char *p = inputbuffer;
  int line = linenr;
  while ((*p != '\0') && (line > 1)) {
    line -= (*p == '\n');
    p++;
  }
  printf("line %3d: ", linenr);
  while ((*p != '\0') && (*p != '\n')) {
    putchar(*p);
    p++;
  }
  putchar('\n');
}
 
void finalizeLexer() {
  free(inputbuffer);
  yylex_destroy();
}
 
