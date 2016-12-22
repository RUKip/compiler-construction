#ifndef HASHTAB_H
#define HASHTAB_H

typedef enum Type {
	INTEGER = 42,
	REAL,
} Type;

void initSymbolTable(void);
void freeSymbolTable(void);
void *lookupSymbol(char *strtabEntry);
void insertSymbol(char *strtabEntry, void *ptrToData, unsigned int size);
void removeSymbol(char *strtabEntry);

#endif