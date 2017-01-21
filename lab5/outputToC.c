#include <stdio.h>
#include <stdlib.h>

FILE *f;
char* currentFunction;

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

void outputFunctionName(int type, char* name){
	currentFunction = name;
	printType(type);
	fprintf(f, "%s (", name);
}

void outputArgument(){

}
