#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include <ctype.h>


void help(){
    printf("Usage:\n");
    printf("	tp1 -h\n");
    printf("	tp1 -V\n");
    printf("	tp1 -i in_file -o out_file\n");
    printf("Options:\n");
    printf("	-V, --version	Print version and quit.\n");
    printf("	-h, --help	Print this information and quit.\n");
    printf("	-i, --input	Specify input stream/file, '-' for stdin.\n");
    printf("	-o, --output	Specify output stream/file, '-' for stdout.\n");
    printf("Examples:\n");
    printf("	tp1 < in.txt > out.txt\n");
    printf("	cat in.txt | tp1 -i - > out.txt\n");
}

bool strings_are_equal(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

#define HELP "-h"
#define VERSION "-v"
#define INPUT "-i"
#define OUTPUT "-o"
#define STD "-"
#define BUFFER_SIZE 1000
#define MAX_NUMS 100


void get_numbers(char* line, int* vector);

int get_length(char* line){
    char buffer[BUFFER_SIZE];
    strncpy(buffer, line, BUFFER_SIZE);
    char* ptr;
    int counter = 0;
    ptr = strtok (buffer, " ");
    while (ptr != NULL && *ptr != '\n'){
        counter++;
        ptr = strtok (NULL, " ");
    }

    printf("Counter: %d\n", counter);
    return counter;
}

void get_numbers(char* line, int* vector) {
    char* numString;
    int counter = 0;
    numString = strtok (line, " ");
    while (numString != NULL && *numString != '\n'){
        int num = atoi(numString);
        vector[counter] = num;
        counter++;
        numString = strtok(NULL, " ");
    }
}


void read_file(FILE* stream){
    char* line;
    size_t bufSize = BUFFER_SIZE;
    while(getline(&line, &bufSize, stream) != -1){
        int vector[MAX_NUMS];
        memset(vector, 0, sizeof(vector));
        int actualVectorLength = get_length(line);
        get_numbers(line, vector);
        //ordenar

        //merge del assembler
        //merge(vector, actualVectorLength);

        for (int i=0; i<actualVectorLength; i++){
            printf("%d ", vector[i]);
        }
        printf("\n");
    }
    printf("\n");
}


/*void menejo_de_error(char* mensaje){
	fprintf(stderr, "%s \n", mensaje);
    exit(EXIT_FAILURE);
}*/


int main(int argc, char* argv[]) {

	FILE* in;

	if(argv[1]){
		char* execOption = argv[1];
		if (strings_are_equal(execOption, HELP)){
            help();
        } else if (strings_are_equal(execOption, VERSION)){
          	printf("versión TP1 Organización de computadoras\n");
        } else if (strings_are_equal(execOption, INPUT)){
        	/*if(!argv[2])
        		manejo_de_error("falta el archivo de texto");*/
        	char* in_file = argv[2];
            in = fopen(in_file, "r+");
        } else{
        	printf("parametro invalido");
        	return 0;
        }
        read_file(in);
        //char* out_command;
        //if(argv[3])
        	char* out_command = argv[3];
        if(strings_are_equal(execOption, out_command)){
        	/*if (!argv[4])
                manejo_de_error("archivo output invalido\n");*/
        	char* out_file = argv[4];
        	if (!strings_are_equal(out_file, STD)){
         	   printf("Salida por archivo");
    		}
    	}

    //stdin
	}else{
		in = stdin;
		read_file(in);
	}

	return 0;
}


