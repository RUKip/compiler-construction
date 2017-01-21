#include <stdio.h>
#include <stdlib.h>

FILE *f;
char* currentFunction;
int tempCount = 0;
int lastTemp;

typedef struct argument {
  char *name;  
  int type;  
} argument;

argument* arguments1;
int argSize1, argIndex1;

void initStoredArguments(){
	argSize1=10;
	argIndex1=0;
	arguments1 = malloc(argSize1*sizeof(argument)); 
}

void copyArguments(argument* new, argument* old) {
    int i;
    for (i = 0; i < argSize1; i++) {
        new[i] = old[i];
    }
}

void storeArgument(int type, char* name) {
    if((argSize1-1) == argIndex1){
        argument* temp = malloc(argSize1*2*sizeof(argument));    
        copyArguments(temp, arguments1);
        argSize1 *= 2;
        free(arguments1);
        arguments1 = temp;    
    } 
    argument arg;
    arg.name = name;
    arg.type = type;
    arguments1[argIndex1] = arg;
    argIndex1++;
}

void printType(int type){
	switch(type){	
	case -1:
		fprintf(f, "void ");	
	case 265:
		fprintf(f, "int ");	
		break;
	case 266:
		fprintf(f, "float ");
		break;	
	case 267:
		fprintf(f, "int[] ");
		break;	
	case 268:
		fprintf(f, "float[] ");
		break;
	}
}

void initializeOutput(){
	f = fopen("Ouput.c", "w+");
}

void outputDeclaration(int type, char* name){
	printType(type);
	fprintf(f, "%s;\n", name);
}

void outputArgument(int type, char* ident, int isLastElement){
	printType(type);
	fprintf(f, "%s", ident);
	if (!isLastElement) {
		fprintf(f, ", ");
	}
}

void outputFunctionName(int type, char* name){
	currentFunction = name;
	fprintf(f, "\n");
	printType(type);
	fprintf(f, "%s (", name);
	int i;
	for (i = 0; i < argIndex1; i++) {
		outputArgument(arguments1[i].type, arguments1[i].name, (i == argIndex1-1));
	}
	fprintf(f, "){\n");
	free(arguments1);
}

void outputTempValue(int type){
	printType(type);
	fprintf(f, "t%d = ", tempCount);
	tempCount++;
}

void outputString(char* string){
	fprintf(f, "%s ", string);	
}

void outputLastTemp(){
	fprintf(f, "t%d ", lastTemp);
	lastTemp = tempCount-1;
}

void outputEnd(){
	fprintf(f, ";\n");
	lastTemp = tempCount-1;
}


int tempSize1, tempIndex1;
int* tempList;

void initTempList(){
	tempSize1=10;
	tempIndex1=0;
	tempList = malloc(tempSize1*sizeof(int)); 
}

void copyTempList(int* new, int* old) {
    int i;
    for (i = 0; i < tempSize1; i++) {
        new[i] = old[i];
    }
}

void storeToTempList() {
    if((tempSize1-1) == tempIndex1){
        int* temp = malloc(tempSize1*2*sizeof(int));    
        copyTempList(temp, tempList);
        tempSize1 *= 2;
        free(tempList);
        tempList = temp;    
    } 
    tempList[tempIndex1] = lastTemp;
    tempIndex1++;
}

void outputTempList(){
	fprintf(f, "(");
	int i;
	for(i = 0; i<tempIndex1; i++){
		fprintf(f, "t%d", tempList[i]);
		if(tempIndex1-1 != i){
			fprintf(f, ", ");
		}
	}
	fprintf(f, ")");
	free(tempList);
}

void outputMain(){
	fprintf(f, "\nint main(void){\n");
}

void outputEndMain(){
	fprintf(f, "return 0; \n }");
}

int getLastTemp(){
	return lastTemp;
}