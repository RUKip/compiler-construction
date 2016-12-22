/* symtab.h: very simple symbol table.
 * author: Arnold Meijster (a.meijster@rug.nl)
 */

#ifndef HASHTAB_H
#define HASHTAB_H

void initSymbolTable(void);
void freeSymbolTable(void);
void *lookupSymbol(char *strtabEntry);
void insertSymbol(char *strtabEntry, void *ptrToData, unsigned int size);
void removeSymbol(char *strtabEntry);

#endif
