#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include <ctype.h>
#include <assert.h>

#define HELP "-h"
#define VERSION "-v"
#define INPUT "-i"
#define OUTPUT "-o"
#define STD "-"
#define BASE 10

int* get_numbers(char* line, size_t capacity, int* amountOfNumbers);
void write_vector(FILE* out_stream, int* vector, int length);
void help();
bool strings_are_equal(char* str1, char* str2);
bool check_if_there_are_letters(char* buffer);
bool endOfLine(char* str);
int merge(int* array, int size);



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

bool check_if_there_are_letters(char* buffer){
    char* endptr = NULL;
    strtol(buffer, &endptr, BASE);

    //chequeo el resultado de strtol, que detecta los caracteres
    bool lettersFound = *endptr != '\0' || endptr == buffer;
    return lettersFound;
}

bool strings_are_equal(char* str1, char* str2){
    if (str1 != NULL && str2 != NULL)
        return strcmp(str1, str2) == 0;

    return false;
}

void check_malloc(int size, void* ptr){
    if (size != 0 && ptr == NULL){
        fprintf(stderr, "Fallo al alocar memoria");
        exit(-1);
    }
}

bool endOfLine(char* str){
    return *str == '\n' || strings_are_equal(str, "\\n");
}

int* get_numbers(char* line, size_t capacity, int* amountOfNumbers) {
    bool lettersFound = false;
    char* buffer = malloc(capacity * sizeof(char));
    check_malloc(capacity, buffer);

    strncpy(buffer, line, capacity);
    char* ptr;
    ptr = strtok (buffer, " \n");

    int* vector = malloc(capacity * sizeof(int));
    check_malloc(capacity, vector);

    int i = 0;
    for (; ptr != NULL && !endOfLine(ptr) && !lettersFound; i++) {
        if (i == capacity){
            capacity += 10;
            vector = (int*)realloc(vector, capacity*sizeof(int));
        }
        lettersFound = check_if_there_are_letters(ptr);
        if (!lettersFound){
            vector[i] = atoi(ptr);
            ptr = strtok (NULL, " \n");
        }
    }

    if (lettersFound){
        free(vector);
        vector = NULL;
    }

    *amountOfNumbers = i;
    free(buffer);
    return vector;
}

void write_vector(FILE* out_stream, int* vector, int length){
    for (int i=0; i < length; i++) {
        fprintf(out_stream,"%d ", vector[i]);
    }
    fprintf(out_stream,"\n");
}

void read_file(FILE* in_stream, FILE* out_stream){
    char* line = NULL;
    size_t bufSize = 0;
    bool letterFound = false;
    bool error = false;

    while(!letterFound && !error && (bufSize = getline(&line, &bufSize, in_stream)) != -1){
        if (bufSize != 1 || line[0] != ' '){					//si es un espacio vacio leo la siguiente linea.
            int amountOfNumbers = 0;
            int *vector = get_numbers(line, bufSize + 1,        //sumo 1 para evitar una lectura invalida en strtok
                                      &amountOfNumbers);

            letterFound = (vector == NULL);
            if (!letterFound) {
                int return_value = merge(vector, amountOfNumbers);
                if (return_value == 0) {
                    write_vector(out_stream, vector, amountOfNumbers);
                    free(vector);
                } else {
                    error = true;
                    fprintf(stderr, "Ocurrió un error en la función de ordenamiento, se procede al cierre");
                }
            } else {
                fprintf(stderr, "Se encontro una letra en el programa y se procede al cierre ordenado\n");
            }
        }
    }
    free(line);
}


int main(int argc, char* argv[]) {
    FILE* in = stdin;
    FILE* out = stdout;

    char* execOption = argv[1];
    if (execOption != NULL) {
        if (strings_are_equal(execOption, HELP)) {
            help();
            return 0;
        } else if (strings_are_equal(execOption, VERSION)) {
            printf("versión TP1 Organización de computadoras\n");
            return 0;
        } else if (strings_are_equal(execOption, INPUT)) {
            char *in_file = argv[2];
            assert(in_file);

            //abro infile
            if (!strings_are_equal(in_file, STD) && (in = fopen(in_file, "r")) == NULL) {
                fprintf(stderr, "Error al abrir el archivo de entrada");
                return -1;
            }

            char *out_command = argv[3];
            char *out_file = argv[4];

            bool presentOutput = strings_are_equal(out_command, OUTPUT);
            bool stdOut = strings_are_equal(out_file, STD);
            if (presentOutput && !stdOut) {
                if ((out = fopen(out_file, "w+")) == NULL) {
                    fprintf(stderr, "Error al abrir el archivo de salida\n");
                    return -1;
                }
            } else if (!presentOutput || out_file == NULL) {
                out = stdout;
            } else {
                fprintf(stderr,"Las opciones para el out command son '-o' o nada\n");
                return -1;
            }
        } else {
            fprintf(stderr, "Parametro de entrada invalido\n");
            return -1;
        }
    }


    read_file(in, out);

    if (in != stdin)
        fclose(in);

    if (out != stdout){
        fclose(out);
    }

    return 0;
}
