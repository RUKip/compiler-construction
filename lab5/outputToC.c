#include <stdio.h>
#include <stdlib.h>
#include "tempType.h"

FILE *f;
char* currentFunction;
int tempCount = 0;
int labelCount = 0;
struct tempType lastTemp;
int inFunction = 0;

typedef struct argument {
  char *name;  
  int type;  
} argument;

argument* arguments1;
int argSize1, argIndex1;

void outputString(char* string){
	fprintf(f, "%s ", string);	
}

void outputEnd(){
	fprintf(f, ";\n");
	lastTemp.temp = tempCount-1;
	if(inFunction) fprintf(f, "\t");
}

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

void storeArgument(tempType t, char* name) {
    int type = t.type;
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

void printType(tempType t){
	int type = t.type;
	switch(type){	
	case -1:
		fprintf(f, "void ");
		break;
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

void outputDeclaration(tempType t, char* name){
	printType(t);
	fprintf(f, "%s", name);
	outputEnd();
}

void outputArgument(int type, char* ident, int isLastElement){
	struct tempType t;
	t.type = type;
	printType(t);
	fprintf(f, "%s", ident);
	if (!isLastElement) {
		fprintf(f, ", ");
	}
}

void outputFunctionName(tempType t, char* name){
	currentFunction = name;
	fprintf(f, "\n");
	printType(t);
	fprintf(f, "%s (", name);
	int i;
	for (i = 0; i < argIndex1; i++) {
		outputArgument(arguments1[i].type, arguments1[i].name, (i == argIndex1-1));
	}
	fprintf(f, "){\n");
	free(arguments1);
	inFunction = 1;
	fprintf(f, "\t");
}

void endFunction() {
	fprintf(f, ";\n");
	lastTemp.temp = tempCount-1;
	outputString("}\n");
	inFunction = 0;
}

void outputTempValue(tempType type){
	printType(type);
	fprintf(f, "t%d = ", tempCount);
	tempCount++;
}

void outputLastTemp(){
	fprintf(f, "t%d ", lastTemp.temp);
	lastTemp.temp = tempCount-1;
}

int tempSize1, tempIndex1;
tempType* tempList;

void initTempList(){
	tempSize1=10;
	tempIndex1=0;
	tempList = malloc(tempSize1*sizeof(tempType)); 
}

void copyTempList(tempType* new, tempType* old) {
    int i;
    for (i = 0; i < tempSize1; i++) {
        new[i] = old[i];
    }
}

void storeToTempList(int type) {
    if((tempSize1-1) == tempIndex1){
        tempType* temp = malloc(tempSize1*2*sizeof(tempType));    
        copyTempList(temp, tempList);
        tempSize1 *= 2;
        free(tempList);
        tempList = temp;    
    } 
    lastTemp.type = type;
    tempList[tempIndex1] = lastTemp;
    tempIndex1++;
}

void outputTempList(){
	fprintf(f, "(");
	int i;
	for(i = 0; i<tempIndex1; i++){
		fprintf(f, "t%d", tempList[i].temp);
		if(tempIndex1-1 != i){
			fprintf(f, ", ");
		}
	}
	fprintf(f, ")");
	free(tempList);
}

void outputPrintf(){
	outputString("printf(");
	int i;
	outputString("\"");
	for(i = 0; i<tempIndex1; i++){
		switch(tempList[i].type){
		case 265:
			fprintf(f, "%%d ");	
			break;
		case 266:
			fprintf(f, "%%f ");
			break;	
		}	
	}
	outputString("\\n\"");
	outputString(",");
	for(i = 0; i<tempIndex1; i++){
		if(tempIndex1-1 == i){
			fprintf(f, "t%d", tempList[i].temp);
		}else{
			fprintf(f, "t%d, ", tempList[i].temp);
		}
	}	
	outputString(")");
	outputEnd();
}

char** variables;
int varIndex, varSize;

void initVarList(){
	varSize=10;
	varIndex=0;
	variables = malloc(varSize*sizeof(char*)); 
}

void copyVarList(char** new, char** old) {
    int i;
    for (i = 0; i < varIndex; i++) {
        new[i] = old[i];
    }
}

void addVar(char* variable){
	if((varSize-1) == varIndex){
		char** temp = malloc(varSize*2*sizeof(char*));    
		copyVarList(temp, variables);
		varSize *= 2;
		free(variables);
		tempList = temp;    
	}
	variables[varIndex] = variable;
	varIndex++;
}	

void freeVariables(){
	free(variables);
}

void outputScanf(){
	outputString("scanf(");
	int i;
	outputString("\"");
	for(i = 0; i<tempIndex1; i++){
		switch(tempList[i].type){
		case 265:
			fprintf(f, "%%d ");	
			break;
		case 266:
			fprintf(f, "%%f ");
			break;	
		}	
	}
	outputString("\\n\"");
	outputString(",");
	for(i = 0; i<varIndex; i++){
		if(tempIndex1-1 == i){
			fprintf(f, "%s", variables[i]);
		}else{
			fprintf(f, "%s, ",  variables[i]);
		}
	}	
	outputString(")");
	outputEnd();
}

void outputMulop(char* operator){
	if(operator[0] == 'D' || operator[0] == 'd'){ //function can only return MULOP operator so no need for full matching
		fprintf(f, " / ");
	}else if(operator[0] == 'M' || operator[0] == 'm'){
		fprintf(f, " %% ");
	}else{
		fprintf(f, " %s ", operator);
	}
}

void outputMain(){
	fprintf(f, "\nint main(void){\n\t");
	inFunction = 1;
}

void outputEndMain(){
	inFunction = 0;
	fprintf(f, "return 0; \n }");
}

int getLastTemp(){
	return lastTemp.temp;
}

void outputOldTemp(int temp) {
	fprintf(f, "t%d ", temp);
}

int getLabel(){
	labelCount++;
	return labelCount;
}

void outputLabel(int cnt){
	fprintf(f, "lab%d", cnt);
}