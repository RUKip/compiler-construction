#ifndef HASHTAB_H
#define HASHTAB_H

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  int type;  //265 is INT, 266 is REAL
  struct bucket *next;
} *bucket;

bucket* initSymbolTable();
void freeSymbolTable(bucket* hashtab);
void *lookupSymbol(bucket* hashtab, char *strtabEntry);
void insertSymbol(bucket* hashtab, char *strtabEntry, int type);
void removeSymbol(bucket* hashtab, char *strtabEntry);

#endif