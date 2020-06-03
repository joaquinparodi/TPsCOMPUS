#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include <ctype.h>
#include <assert.h>


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
    if (str1 != NULL && str2 != NULL)
        return strcmp(str1, str2) == 0;

    return false;
}

#define HELP "-h"
#define VERSION "-v"
#define INPUT "-i"
#define OUTPUT "-o"
#define STD "-"
#define BUFFER_SIZE 1000
#define MAX_NUMS 100
#define MAX_NUMS_LENGTH 10
#define STDOUT_MODE 1
#define FILE_MODE 2


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

void write_vector(FILE* out_stream, int* vector, int length, int out_mode){
    if (out_mode == FILE_MODE) {
        for (int i = 0; i < length; i++) {
            int actualValue = vector[i];
            char str[MAX_NUMS_LENGTH];
            memset(str, 0, MAX_NUMS_LENGTH);
            sprintf(str, "%d", actualValue);
            fputs(str, out_stream);
            fputc(' ', out_stream);
        }
        fputc('\n', out_stream);
    } else {
        for (int i=0; i < length; i++) {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }
}

void read_file(FILE* in_stream, FILE* out_stream, int out_mode){
    char line[BUFFER_SIZE];
    char* ptr = line;
    size_t bufSize = BUFFER_SIZE;

    while(getline(&ptr, &bufSize, in_stream) != -1){
        int vector[MAX_NUMS];
        memset(vector, 0, sizeof(vector));
        int actualVectorLength = get_length(line);
        get_numbers(line, vector);

        //merge del assembler
        merge(vector, actualVectorLength);

        write_vector(out_stream, vector, actualVectorLength, out_mode);
    }
}


int main(int argc, char* argv[]) {
	FILE* in = NULL;
	FILE* out = NULL;
	int out_mode = 0;

	char* execOption = argv[1];
	if (execOption != NULL) {
		if (strings_are_equal(execOption, HELP)){
            help();
            return 0;
        } else if (strings_are_equal(execOption, VERSION)){
          	printf("versión TP1 Organización de computadoras\n");
          	return 0;
        } else if (strings_are_equal(execOption, INPUT)){
            char* in_file = argv[2];
            assert(in_file);

            //abro infile
		    if ((in = fopen(in_file, "r")) == NULL){
                fprintf(stderr, "Error al abrir el archivo de entrada");
                return -1;
		    }

            char* out_command = argv[3];
            char* out_file = argv[4];

            bool presentOutput = strings_are_equal(out_command, OUTPUT);
            bool stdOut = strings_are_equal(out_file, STD);
            if (presentOutput && !stdOut){
                if ((out = fopen(out_file, "w+")) == NULL){
                    fprintf(stderr, "Error al abrir el archivo de salida\n");
                    return -1;
                }
                out_mode = FILE_MODE;
                //si el out command es nulo o si quiere que salga por entrada estandar
            } else if (out_command == NULL || (presentOutput && stdOut)){
                printf("Mando out a salida estandar\n");
                out = stdout;
                out_mode = STDOUT_MODE;
            } else {
                fprintf(stderr, "Las opciones para el out command son '-o' o nada\n");
                return -1;
            }
        } else {
        	fprintf(stderr, "Parametro de entrada invalido");
        	return -1;
        }
	} else {
		in = stdin;
		out = stdin;
		out_mode = STDOUT_MODE;
	}

	read_file(in, out, out_mode);
	return 0;
}


