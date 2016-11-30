#include <stdio.h>
#include <stdlib.h>
#include "intset.h"

/* Do not change EPSILON! There are 128 ASCII characters. */
#define EPSILON 128

typedef unsigned int State;

typedef struct nfa {
  unsigned int nstates;  /* number of states                          */
  State start;           /* number of the start state                 */
  intSet final;          /* set of final (accepting) states           */
  intSet **transition;   /* transition: state x char -> set of states */
} nfa;


typedef struct dfa {
  unsigned int nstates;  /* number of states                          */
  State start;           /* number of the start state                 */
  intSet final;          /* set of final (accepting) states           */
  int **transition;      /* transition: state x char -> set of states */
} dfa;

static void *safeMalloc(unsigned int sz) {
  void *ptr = malloc(sz);
  if (ptr == NULL) {
    fprintf(stderr, "Fatal error: malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void *safeRealloc(void *ptr, unsigned int sz) {
  ptr = realloc(ptr, sz);
  if (ptr == NULL) {
    fprintf(stderr, "Fatal error: realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/****** NFA **************************************************/

nfa makeNFA(int nstates) {
  nfa n;
  unsigned int c;
  State s;
  n.nstates = nstates;
  n.start = 0;   /* default start state */
  n.final = makeEmptyIntSet();
  n.transition = safeMalloc(nstates*sizeof(intSet *));
  for (s=0; s < nstates; s++) {
    n.transition[s] = safeMalloc(129*sizeof(intSet));
    for (c=0; c <= EPSILON; c++) {
      n.transition[s][c] = makeEmptyIntSet();
    }
  }
  return n;
}

void freeNFA(nfa n) {
  unsigned int c;
  State s;
  freeIntSet(n.final);
  for (s=0; s < n.nstates; s++) {
    for (c=0; c <= EPSILON; c++) {
      freeIntSet(n.transition[s][c]);
    }
    free(n.transition[s]);
  }
  free(n.transition);
}

nfa readNFA(char *filename) {
  FILE *f;
  nfa n;
  unsigned int nstates;
  State state;

  f = fopen(filename, "r");
  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  if (!fscanf(f, "%u\n", &nstates)) {
    fprintf(stderr, "Error: 1st line must be the number of states\n");
    exit(EXIT_FAILURE);
  }
  n = makeNFA(nstates);
  if (!fscanf(f, "%u\n", &n.start)) {
    fprintf(stderr, "Error: 2nd line must be the number of the start state\n");
    exit(EXIT_FAILURE);
  }
  n.final = readIntSetFromFile(f);
  /* read transitions */
  while (fscanf(f, "%u\n", &state) == 1) {
    int c;
    do {
      c = getc(f);
    } while ((c == ' ') || (c == '\t') || (c == '\n'));
    /* c can be a quote ('), 'e' (from eps), or '#' (ascii number) */
    switch (c) {
    case '\'':
      c = fgetc(f);
      fgetc(f);  /* skip closing quote */
      break;
    case 'e': 
      fgetc(f);  /* skip 'p' */
      fgetc(f);  /* skip 's' */
      c = EPSILON;
      break;
    case '#':
      if(!fscanf(f, "%d", &c)) {
        fprintf(stderr, "Error: # must be followed by an unsigned integer\n");
	exit(EXIT_FAILURE);
      }
      break;
    default: 
      fprintf(stderr, "Syntax error in automata file   %c\n", c);
      exit(EXIT_FAILURE);
    }
    n.transition[state][c] = readIntSetFromFile(f);
  }
  fclose(f);
  return n;
}

void saveNFA(char *filename, nfa n) {
  FILE *f;
  unsigned int c;
  State state;

  f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%d\n%d\n", n.nstates, n.start);
  fprintlnIntSet(f, n.final);
  for (state = 0; state < n.nstates; state++) {
    for (c=0; c<= EPSILON; c++) {
      if (!isEmptyIntSet(n.transition[state][c])) {
        fprintf(f, "%d ", state);
        if (c == EPSILON) {
          fprintf(f, "eps ");
	} else {
          if (c > ' ') {
	    fprintf(f, "'%c' ", c); 
	  } else {
	    fprintf(f, "#%d ", c); 
	  }
	}
        fprintlnIntSet(f, n.transition[state][c]);
      }
    }
  }
  fclose(f);
}

/****** DFA **************************************************/

#define NOTRANSITION (-1)

void freeDFA(dfa d) {
  State s;
  freeIntSet(d.final);
  for (s=0; s < d.nstates; s++) {
    free(d.transition[s]);
  }
  free(d.transition);
}

void saveDFA(char *filename, dfa d) {
  FILE *f;
  unsigned int c;
  State state;

  f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%d\n%d\n", d.nstates, d.start);
  fprintlnIntSet(f, d.final);
  for (state = 0; state < d.nstates; state++) {
    for (c=0; c < 128; c++) {
      if (d.transition[state][c] != NOTRANSITION) {
	if (c > ' ') {
	  fprintf(f, "%u '%c' %u\n", state, c, d.transition[state][c]);
	} else {
	  fprintf(f, "%u #%d %u\n", state, c, d.transition[state][c]);
	}
      }
    }
  }
  fclose(f);
}

/***********************************************************/


void tempIntset(nfa automaton, State s, intSet* closure){
  intSet temp = copyIntSet(automaton.transition[s][EPSILON]);
  int n;
  while(!isEmptyIntSet(temp)){
    n = chooseFromIntSet(temp);
    if(!isMemberIntSet(n, *closure)){
      insertIntSet(n, closure);
      tempIntset(automaton, n, closure);
    } 
    deleteIntSet(n, &temp);
  }
}

/* returns the EPSILON closure for a given state */
intSet epsilonClosureState(nfa automaton, State s) {
  intSet closure = makeEmptyIntSet();
  insertIntSet(s, &closure);
  tempIntset(automaton, s, &closure);
  return closure;
}

/* returns the EPSILON closure for a set of states */
intSet epsilonClosureStateSet(nfa automaton, intSet s) {
  /* implement the body of this function yourself */
  intSet closure = makeEmptyIntSet();
  return closure;
}

/* DFA construction */
dfa nfa2dfa(nfa n) {
  /* implement the body of this function yourself */
  dfa d;
  d.nstates = 0;
  d.start = 0;
  d.final = makeEmptyIntSet();
  d.transition = NULL;
  return d;
}

/* minimal DFA construction using Brzozowski’s algorithm */
dfa nfa2minimalDFA(nfa n) {
  /* implement the body of this function yourself */
  dfa d;
  d.nstates = 0;
  d.start = 0;
  d.final = makeEmptyIntSet();
  d.transition = NULL;
  return d;
}

int main (int argc, char **argv) {
  nfa n;
  dfa d;
  State s;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <nfa file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  n = readNFA(argv[1]);
   saveNFA("out.nfa", n);

#if 1
  /* code for testing epsilonClosureState */
  for (s=0; s < n.nstates; s++) {
    intSet epsclosure = epsilonClosureState(n, s);
    printf("epsclosure(%u)=", s);
    printlnIntSet(epsclosure);
    freeIntSet(epsclosure);
  }
#endif
  
  
#if 0
  /* code for testing nfa2dfa */
  d = nfa2dfa(n);
  saveDFA("out.dfa", d);
  freeDFA(d);
#endif

#if 0
  /* code for testing nfa2minimalDFA */
  d = nfa2minimalDFA(n);
  saveDFA("minimal.dfa", d);
  freeDFA(d);
#endif

  freeNFA(n);
  return EXIT_SUCCESS;
}