#include <stdio.h>
#include <stdlib.h>

int yylex(); 
int nextToken;

void parseEtail();
void parseExpr();
void parseF();
void parseN();
void parseN2();
void parseTtail();
void parseTerm();

int PLUS_TOKEN = 43;
int MIN_TOKEN = 45;
int MUL_TOKEN = 42;
int DIV_TOKEN = 47;
int N_TOKEN = 94;
int L_BRACKET = 40;
int R_BRACKET = 41;

int match(int token) {
  char* str[1];
  str[0] = token;
  printf("nextToken token is: %s\n", nextToken);
  if (strcmp(str,nextToken) != 0) {
    return 0; /* no match */
  }
  printf("token matched\n");
  nextToken = yylex();
  return 1; /* match */
}

void parseExpr(){
  printf("parsing T\n");
  parseTerm();
  printf("parsing Etail\n");
  parseEtail();
}


void parseEtail(){
    while(match(PLUS_TOKEN)){
      printf("parsing T after +\n");
      parseTerm();
      parseEtail();
      return;
    }
    while(match(MIN_TOKEN)){
      printf("parsing T after -\n");
      parseTerm();
      parseEtail();
      return;
    }
    printf("parsing empty after Etail\n");
    return;
}

void parseTerm(){
  printf("parsing N\n");
  parseN();
  printf("parsing Ttail\n");
  parseTtail();
}

void parseTtail(){
  while(match(MUL_TOKEN)){
      parseN();
      parseTtail();
      return;
    }
    while(match(DIV_TOKEN)){
      parseN();
      parseTtail();
      return;
    }
    return;
}

void parseN(){
    printf("parsing F\n");
   parseF();
   printf("parsing N2\n");
   parseN2();
}

void parseN2(){
    while(match(N_TOKEN)){
      printf("parsing N after ^\n");
      parseN();
    }
    printf("parsing Empty\n");
}

void parseF(){
    while(match(L_BRACKET)){
      parseExpr();
      if(!match(R_BRACKET)){
	printf("Syntax error\n"); 
	exit(-1);
      }
      return;
    }
    
    printf("%s\n", nextToken);
    nextToken = yylex();
}

int main(int argc, char *argv[]) {
  nextToken = yylex();
  parseExpr();
  return 0;
}
