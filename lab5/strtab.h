#ifndef STRTAB_H
#define STRTAB_H

/* strtab.h: very simple string table.
 * author: Arnold Meijster (a.meijster@rug.nl)
 */

char *insertStringTable(char *str);
char *lookupStringTable(char *str);
void freeStringTable();
void showStringTable();

#endif
