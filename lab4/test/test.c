
typedef enum Type {
	INTEGER = 42,
	REAL,
} Type;

typedef struct bucket {
  char *key;   /* pointer to entry in the string table */
  enum Type type;  
} *bucket;

int linearSearch(struct bucket b) {
  return b.type;
}


void main(){
	struct bucket b;
	b.key = "test";
	b.type = INTEGER;
int a = linearSearch(b);
	printf("%d\n",a);
}
