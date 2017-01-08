#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE 97   /* choose a small prime */

//makeFile has symtab.c included in makefile to solve undifned refrences, might have errors if acting weird

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  int type;  //265 is INT, 266 is REAL
  struct bucket *next;
} *bucket;

bucket* initSymbolTable() {
  unsigned int i;
  bucket* hashtab = malloc(sizeof(bucket)*TABSIZE);
  for (i=0; i < TABSIZE; i++) {
    hashtab[i] = NULL;
  }
  return hashtab;
}

void freeSymbolTable(bucket* hashtab) {
  unsigned int i;
  for (i=0; i < TABSIZE; i++) {
    while (hashtab[i] != NULL) {
      bucket b = hashtab[i];
      hashtab[i] = hashtab[i]->next;
      free(b);
    }
  }
}

static unsigned int hash(char *str) {
  /* famous hash function: google for Jenkins' hash function */
  unsigned int i, hash = 0, len = strlen(str);
  for(i = 0; i < len; ++i) {
    hash += str[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash%TABSIZE;
}

int linearSearch(char *strtabEntry, bucket b) {
  while (b != NULL) {
    if (strtabEntry == b->key) {
      return b->type;
    }
    b = b-> next;
  }
  return 0;
}

int lookupSymbol(bucket* hashtab, char *strtabEntry) {
  return linearSearch(strtabEntry, hashtab[hash(strtabEntry)]);
}

void insertSymbol(bucket* hashtab, char *strtabEntry, int type) {
  unsigned int h = hash(strtabEntry);
  bucket nxt = hashtab[h];
  hashtab[h] = malloc(sizeof(struct bucket));
  hashtab[h]->next = nxt;
  hashtab[h]->key = strtabEntry;  /* not a string copy! */
  hashtab[h]->type = type;
}

void removeSymbol(bucket* hashtab, char *strtabEntry) {
  unsigned int h = hash(strtabEntry);
  bucket b = hashtab[h];
  if (linearSearch(strtabEntry, hashtab[h]) == 0) {
    fprintf(stderr, "Error in removeSymbol(%s): not found\n", strtabEntry);
    exit(EXIT_FAILURE);
  }
  if (b->key == strtabEntry) {
    hashtab[h] = b->next;
  } else {
    bucket previous = b;
    b = b->next;
    while (b->key != strtabEntry) {
      previous = b;
      b = b->next;
    }
    previous->next = b->next;
  }
  free(b);
}

