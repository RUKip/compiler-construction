int stackSize;
int stackIndex;
char** stack;

char* pop() {
    if(stackIndex == -1){
        return 0;
    }
    stackIndex--;
    return stack[stackIndex + 1];
}

void copy(char** new, char** old) {
    int i;
    for (i = 0; i < stackSize; i++) {
        new[i] = old[i];
    }
}

void push(char* c) {
    if((stackSize-1) == stackIndex){
        char** temp = malloc(stackSize*2*sizeof(char*));    
        copy(temp, stack);
        stackSize *= 2;
        free(stack);
        stack = temp;    
    } 
    stack[stackIndex+1] = c;
    stackIndex++;
}

void initStack() {
	stackSize = 10;
	stackIndex = -1;
	stack = malloc(stackSize*sizeof(char*));
}

void freeStack() {
	free(stack);
}

int isEmpty() {
	return stackIndex == -1;
}
