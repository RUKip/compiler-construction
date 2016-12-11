#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int value;
int yylex(); 
int nextToken;

void parseEtail();
void parseExpr();
void parseF();
void parseN();
void parseN2();
void parseTtail();
void parseTerm();

int PLUS_TOKEN = 1;
int MIN_TOKEN = 2;
int MUL_TOKEN = 3;
int DIV_TOKEN = 4;
int N_TOKEN = 5;
int L_BRACKET = 6;
int R_BRACKET = 7;
int NUMBER = 8;

int match(int token) {
  if(nextToken == NULL) exit(0); //TODO: close tree here, instead of exit
  printf("nextToken token is: %d\n", nextToken);
  if (nextToken != token) {
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
    while(match(MIN_TOKEN)){ //TODO: strange things happen to -
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
    
    printf("%d\n", value);
    nextToken = yylex();
}

int main(int argc, char *argv[]) {
  nextToken = yylex();
  //TODO: make tree here
  parseExpr(); //TODO: should still fill tree
  //TODO: print tree here
  return 0;
}
