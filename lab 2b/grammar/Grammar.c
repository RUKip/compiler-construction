#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"

int value;
int yylex(); 
int nextToken;

typedef struct ExpTreeNode *ExpTree;
typedef int TokenType;
typedef int Token;

typedef struct ExpTreeNode {
  TokenType tt;
  Token t;
  ExpTree left;
  ExpTree right;
} ExpTreeNode;

ExpTree parseEtail();
ExpTree parseExpr();
ExpTree parseF();
ExpTree parseN();
ExpTree parseN2();
ExpTree parseTtail();
ExpTree parseTerm();

int PLUS_TOKEN = 1;
int MIN_TOKEN = 2;
int MUL_TOKEN = 3;
int DIV_TOKEN = 4;
int N_TOKEN = 5;
int L_BRACKET = 6;
int R_BRACKET = 7;
int NUMBER = 8;



/* creates a new node for an expression tree */
ExpTree newExpTreeNode(TokenType tt, Token t, ExpTree tL, ExpTree tR) {
  ExpTree new = malloc(sizeof(ExpTreeNode));
  assert (new!=NULL);
  new->tt = tt;
  new->t = t;
  new->left = tL;
  new->right = tR;
  return new;
}

int match(int token) {
  if (nextToken != token) {
    return 0; /* no match */
  }
  nextToken = yylex();
  return 1; /* match */
}

ExpTree parseExpr(){
  ExpTree l = parseTerm();
  return parseEtail(l);
}


ExpTree parseEtail(ExpTree l){
	if(match(PLUS_TOKEN)){
		ExpTree r = parseTerm();
		ExpTree p = newExpTreeNode(2, PLUS_TOKEN, l, r);
		return parseEtail(p);
	}
	if(match(MIN_TOKEN)){
		ExpTree r = parseTerm();
		ExpTree p = newExpTreeNode(2, MIN_TOKEN, l, r);
		return parseEtail(p);
	}
	return l;
}

ExpTree parseTerm(){
  ExpTree n = parseN();
  return parseTtail(n);
}

ExpTree parseTtail(ExpTree l){
  if(match(MUL_TOKEN)){
		ExpTree r = parseN();
		ExpTree p = newExpTreeNode(2, MUL_TOKEN, l, r);
		return parseTtail(p);
	}
	if(match(DIV_TOKEN)){
		ExpTree r = parseN();
		ExpTree p = newExpTreeNode(2, DIV_TOKEN, l, r);
		return parseTtail(p);
	}
	return l;
}

ExpTree parseN(){
   ExpTree l = parseF();
   ExpTree p = parseN2();
   if (p != NULL) {
     p->left = l;
     return p;
   } else {
     return l;
   }
}

ExpTree parseN2(){
	if(match(N_TOKEN)){
		ExpTree n = parseN();
		return newExpTreeNode(2, N_TOKEN, NULL, n);
	}
	return NULL;
}

ExpTree parseF(){
	if(match(L_BRACKET)){
		ExpTree e;
		e = parseExpr();
		if(!match(R_BRACKET)){
			printf("Error expected ). Abort\n"); 
			exit(-1);
		}
		return e;
	}
	if(nextToken != 8) {
		printf("Error, expected ( Expression ) or a number. Abort\n");
		exit(-1);
	}
	ExpTree num = newExpTreeNode(1, value, NULL, NULL);
	nextToken = yylex();
	return num;
}

void printPostfix(ExpTree t){
  if(NULL == t){
    return;
  }
  printPostfix(t->left);
  printPostfix(t->right);
  if(t->tt == 1){ //Number
    printf("%d ", t->t);
  }else if(t->tt == 2){ //operator
    switch(t->t){
			case 1:
				printf("+ ");
				break;
      case 2:
				printf("- ");
				break;
      case 3:
				printf("* ");
      case 4:
				printf("/ ");
      case 5:
				printf("^ "); 
    }
  }
}

void freeExpTree(ExpTree t) {
  if (t == NULL) {
    return;
  }
  freeExpTree(t->left);
  freeExpTree(t->right);
  free(t);
}

int main(int argc, char *argv[]) {
  nextToken = yylex();
  ExpTree tree = parseExpr();
  printPostfix(tree);
  putchar('\n');
  freeExpTree(tree);
  yylex_destroy();
  return 0;
}
