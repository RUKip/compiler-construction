/* symtab.c: very simple symbol table.
 * author: Arnold Meijster (a.meijster@rug.nl)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE 97   /* choose a small prime */

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  void *data;
  struct bucket *next;
} *bucket;

static bucket hashtab[TABSIZE];

void initSymbolTable(void) {
  unsigned int i;
  for (i=0; i < TABSIZE; i++) {
    hashtab[i] = NULL;
  }
}

void freeSymbolTable(void) {
  unsigned int i;
  for (i=0; i < TABSIZE; i++) {
    while (hashtab[i] != NULL) {
      bucket b = hashtab[i];
      hashtab[i] = hashtab[i]->next;
      free(b->data);
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

static void *linearSearch(char *strtabEntry, bucket b) {
  while (b != NULL) {
    if (strtabEntry == b->key) {
      return b->data;
    }
    b = b-> next;
  }
  return NULL;
}


void *lookupSymbol(char *strtabEntry) {
  return linearSearch(strtabEntry, hashtab[hash(strtabEntry)]);
}

void insertSymbol(char *strtabEntry, void *ptrToData, unsigned int size) {
  unsigned int h = hash(strtabEntry);
  bucket nxt = hashtab[h];
  hashtab[h] = malloc(sizeof(struct bucket));
  hashtab[h]->next = nxt;
  hashtab[h]->key = strtabEntry;  /* not a string copy! */
  hashtab[h]->data = malloc(size);
  memcpy(hashtab[h]->data, ptrToData, size);
}

void removeSymbol(char *strtabEntry) {
  unsigned int h = hash(strtabEntry);
  bucket b = hashtab[h];
  if (linearSearch(strtabEntry, hashtab[h]) == NULL) {
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
  free(b->data);
  free(b);
}
