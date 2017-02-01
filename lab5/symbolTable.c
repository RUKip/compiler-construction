#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tempType.h"

#define TABSIZE 97

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  int type;  
  struct bucket *next;
  int isFunction;
  int* arguments;
  int size;
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
	  if (b->isFunction) free(b->arguments);
      free(b);
    }
  }
  free(hashtab);
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

int linearFunctionSearch(char *strtabEntry, bucket b) {
  while (b != NULL) {
    if (strtabEntry == b->key) {
      return b->isFunction;
    }
    b = b-> next;
  }
  return 0;
}

int isFunction(bucket* hashtab, char *strtabEntry) {
  return linearFunctionSearch(strtabEntry, hashtab[hash(strtabEntry)]);
}

void copyArray(int* new, int* old, int size) {
    int i;
    for (i = 0; i < size; i++) {
        new[i] = old[i];
    }
}

void linearAddArguments(char *strtabEntry, bucket b, int* arg, int size) {
  while (b != NULL) {
    if (strtabEntry == b->key) {
	  int* temp = malloc(size*sizeof(int));    
	  copyArray(temp, arg, size);   
      b->arguments = temp;
      b->size = size;
      return;
    }
    b = b-> next;
  }
}

void addArguments(bucket* hashtab, char *strtabEntry, int* arg, int size) {
  linearAddArguments(strtabEntry, hashtab[hash(strtabEntry)], arg, size);
}

void insertSymbol(bucket* hashtab, char *strtabEntry, tempType t, int isFunction) {
  int type = t.type;
  unsigned int h = hash(strtabEntry);
  bucket nxt = hashtab[h];
  hashtab[h] = malloc(sizeof(struct bucket));
  hashtab[h]->next = nxt;
  hashtab[h]->key = strtabEntry;  /* not a string copy! */
  hashtab[h]->type = type;
  hashtab[h]->isFunction = isFunction;
  hashtab[h]->arguments = NULL;
  hashtab[h]->size = 0;
}

bucket linearBucketSearch(char *strtabEntry, bucket b) {
  while (b != NULL) {
    if (strtabEntry == b->key) {
      return b;
    }
    b = b-> next;
  }
  return 0;
}

void checkArguments(bucket* hashtab, char *strtabEntry, int* arg, int size) {
	bucket b = linearBucketSearch(strtabEntry, hashtab[hash(strtabEntry)]);
	if(!(b->size == size)) {
		printf("argument count mismatch in %s\n", strtabEntry);
		exit(-1);
	}
	int i;
	for (i = 0; i < size; i++) {
		int type1 = b->arguments[i];
		int type2 = arg[i];
		if((type1 != type2) && !((265 == type2 || type2 == 266) && (265 == type1 || type1 == 266))) {
			printf("argument type mismatch on argument %d in %s\n", i, strtabEntry);
			exit(-1);
		}
	}
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

