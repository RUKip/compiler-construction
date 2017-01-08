/* symtab.c: very simple string table.
 * author: Arnold Meijster (a.meijster@rug.nl)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned tabsize, idx;
char *strtab = NULL;

void initializeStringTable(char* filename){
  //new stuff
  FILE *f = fopen(filename, "r");
  int length;
  if (f == NULL) {
    fprintf(stderr, "Error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 0, SEEK_SET);
  strtab = malloc (length+1);
  fclose (f);
  //end of new stuff
}


static void resize(int sz) {
  if (idx + sz >= tabsize) {
	tabsize = (tabsize == 0 ? 2 : 2*tabsize);
	while (idx + sz >= tabsize) {
	  tabsize *= 2;
	}
	strtab = realloc(strtab, tabsize);
  }
}


char *lookupStringTable(char *str) {
  unsigned i=0, len = strlen(str);
  while (i < idx) {
    unsigned l = strlen(&strtab[i]);
    if ((l == len) && (strcmp(&strtab[i], str) == 0)) {
	  return (&strtab[i]);
	}
	i = i + l + 1;
  }
  return NULL;
}

char *insertStringTable(char *str) {
  char *ptr = lookupStringTable(str);
  if (ptr == NULL) {
    unsigned len = strlen(str);
    resize(len);
    strcpy(&strtab[idx], str);
    ptr = &strtab[idx];
    idx += len + 1;
  }
  return ptr;
}

void showStringTable() {
  unsigned i=0;
  while (i < idx) {
    printf("%p: %s\n", &strtab[i], &strtab[i]);
	i = i + strlen(&strtab[i]) + 1;
  }
}

void freeStringTable() {
  free(strtab);
  strtab = NULL;
  tabsize = 0;
}

