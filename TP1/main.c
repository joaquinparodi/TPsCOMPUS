#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void merge(int* vector, int len_vector);

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




int* to_vector(char* string,int* len){
	int len_string = 0;
	for(int j = 0; (string[j] != '\n');j++){
		if(string[j] == ' '){
            len_string++;
        }
		else if( (isdigit(string[j])==0 && (string[j]!='-'))  || ((string[j]=='-') && isdigit(string[j+1])==0)){
            fprintf( stderr, "ERROR: Uno o más datos del archivo no son válidos, por ejemeplo el simbolo %c no se corresponde con un número.\n", string[j]);
            exit(EXIT_FAILURE);
        }
	}
	int* vector = malloc (sizeof(int)*(len_string));
	if (!vector){
        fprintf(stderr, "ERROR: No se pudo reservar la memoria suficiente.\n");
        exit(EXIT_FAILURE);
	}
	const char sep[2] = " ";
	char *temp;
	temp = strtok(string, sep);
	int i = 0;
	while(strcmp(temp,"\n")!=0){
		vector[i] = atoi(temp);
		i++;
		temp = strtok(NULL, sep);
		if (!temp){
            break;
        }
	}
    *len=i;
	return vector;
}


void print_vector(int* vector,int* len){
    int i=0;
    while (i<*len){
        printf("%i ", vector[i]);
        i++;
    }
    printf("\n");
}


void write_file(int* vector,FILE* dfile,int*len){
    int i=0;
    while(i<*len){
        char numero [10];
        sprintf(numero, "%i", vector[i]);
        fputs(numero,dfile);
        fputs(" ",dfile);
        i++;
    }
    fputs("\n",dfile);
}


void process_line(char* line,FILE* dfile){
    int* len = malloc(sizeof(int));
    if(!len){
        fprintf( stderr, "ERROR: no se pudo reservar memoria para almacenar la longitud del arreglo");
        exit(EXIT_FAILURE);
        }
    int* vector = to_vector(line,len);
    //verifica si la linea esta vacía
    if(*len == 1 && vector[0] == 0){
    	fprintf( stderr, "ERROR: se insertó una línea vacía\n");
        exit(EXIT_FAILURE);
    }
    if (*len>0){
    	merge(vector, *len);
    }
    if (dfile==NULL){
        print_vector(vector,len);
    }
    else{
        write_file(vector,dfile,len);
    }
    free(vector);
    free(len);
}



char* read_line(FILE* file){
    int tam=100;
    char* line= malloc(sizeof(char)*tam);
    if (!line){
        fprintf( stderr, "ERROR: No se pudo reservar memoria suficente para la lectura de la linea");
        exit(EXIT_FAILURE);
    }
    char c;
    int pos=0;
    int count=1;
    c = fgetc(file);
    if (c==EOF){
        return NULL;
    }
    while (c!='\n'){
        if (count==100){
            tam=tam+100;
            line=realloc(line,tam*sizeof(char));
            if (!line){
                fprintf( stderr, "ERROR: No se pudo almacenar la linea correctamente");
                exit(EXIT_FAILURE);
            }
            count=1;
        }
        line[pos]=c;
        pos++;
        count++;
        c = fgetc(file);
    }
    line[pos]=c;
    return line;
}




void process_file(char* filename, char* destination){
    FILE* dfile=NULL;
    if (strcmp(destination,"Unspecified") != 0){
        dfile= fopen(destination,"w+");
        if (!dfile){
            fprintf( stderr, "ERROR: no se ha podido crear el archivo en la ruta especificada\n");
            exit(EXIT_FAILURE);
        }
    }
    FILE* file;
    if (strcmp(filename,"stdin") == 0){
        file=stdin;
    }
    else{
        file = fopen(filename,"r");
    }
    if (!file){
        fprintf( stderr, "ERROR: no se encuentra el archivo de entrada\n");
        exit(EXIT_FAILURE);
    }
    char*line= read_line(file);
    while(line!=NULL){
        process_line(line,dfile);
        free(line);
        line= read_line(file);
    }
    if (strcmp(filename,"stdin") != 0){
        fclose(file);
    }
    if (strcmp(destination,"Unspecified") != 0){
        fclose(dfile);
    }
}




int main(int argc, char *argv[]){

    //stdin salida por pantalla
    if(!argv[1]){
        process_file("stdin","Unspecified");
    }

    //stdin salida por archivo
    else if (strcmp(argv[1],"-o") == 0){
        if (!argv[2]){
        fprintf( stderr, "ERROR: ingrese la ruta del archivo de salida\n");
        exit(EXIT_FAILURE);
        }
        process_file("stdin",argv[2]);
    }

    // version
	else if(strcmp(argv[1],"-V") == 0){
		printf("versión TP1 Organización de computadoras\n");
		return 0;
	}

	//help
	else if(strcmp(argv[1],"-h") == 0){
		help();
		return 0;
	}

	//entrada por archivo y salida por pantalla o por archivo
	else if(strcmp(argv[1],"-i") == 0){
         if(!argv[2]){
            fprintf( stderr, "ERROR: ingrese la ruta del archivo de entrada\n");
            exit(EXIT_FAILURE);
         }

         if (!argv[3]){
            process_file(argv[2],"Unspecified");
         }
		 else if(strcmp(argv[3],"-o") == 0){
            if (!argv[4]){
                fprintf( stderr, "ERROR: ingrese la ruta del archivo de salida\n");
                exit(EXIT_FAILURE);
            }
            process_file(argv[2],argv[4]);
		}

	}

	else{
        fprintf( stderr, "ERROR: uno o más parámetros son incorrectos\n");
        exit(EXIT_FAILURE);

	}

	return 0;
}







//NUEVO MAIN

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"


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
        merge(vector, actualVectorLength);

        for (int i=0; i<actualVectorLength; i++){
            printf("%d ", vector[i]);
        }
        printf("\n");
    }
    printf("\n");
}

void menejo_de_error(char* mensaje){
	fprintf(stderr, "%s \n", mensaje);
    exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]) {

	FILE* in;

	if(argv[1]){
		char* execOption = argv[1];
		if (strings_are_equal(execOption, HELP)){
            help();
        } else if (strings_are_equal(execOption, VERSION)){
          	printf("versión TP1 Organización de computadoras\n");
        } else if (strings_are_equal(execOption, INPUT)){
        	if(!argv[2])
        		manejo_de_error("falta el archivo de texto");
        	char* in_file = argv[2];
            in = fopen(in_file, "r+");
        } else{
        	printf("no es valido", );
        }
        read_file(in);
        char* out_command = argv[3];
        if(strings_are_equal(execOption, out_command)){
        	if (!argv[4])
                manejo_de_error("archivo output invalido\n");
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


