int yylex(); 
int nextToken;

int match(int token) {
  if (nextToken != token) {
    return 0; /* no match */
  }
  nextToken = yylex();
  return 1; /* match */
}


void parseTerm(){
  printf("this worked parsed term, %c", nextToken);
}

/* Function parseExpr
Parses strings in the
language generated by the rule:
<expr> → <term> {(+ |-) <term>} */
void parseExpr(){
  parseTerm();
  while (match('-')||match('+')) {
    parseTerm();
  }
}




int main(int argc, char *argv[]) {
  parseExpr();
  return 0;
}