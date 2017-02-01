#ifndef HASHTAB_H
#define HASHTAB_H

#include "tempType.h"

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  int type;  //265 is INT, 266 is REAL
  struct bucket *next;
} *bucket;

bucket* initSymbolTable();
void freeSymbolTable(bucket* hashtab);
int lookupSymbol(bucket* hashtab, char *strtabEntry);
int isFunction(bucket* hashtab, char *strtabEntry);
void addArguments(bucket* hashtab, char *strtabEntry, int* arg, int size);
void insertSymbol(bucket* hashtab, char *strtabEntry, tempType type, int isFunction);
void checkArguments(bucket* hashtab, char *strtabEntry, int* arg, int size);
void removeSymbol(bucket* hashtab, char *strtabEntry);

#endif
