#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void merge(int* vector, int len_vector);

int* convertir_a_vector(char* cadena){
	int len_cadena = 0;
	for(int j = 0; (cadena[j] != '\0');j++){

		if(cadena[j] == ' '){
            len_cadena++;
        }

		else if( (isdigit(cadena[j])==0 && (cadena[j]!='-'))  || ((cadena[j]=='-') && isdigit(cadena[j+1])==0)){
            fprintf( stderr, "ERROR: Uno o más datos del archivo no son válidos, por ejemeplo el simbolo %c no se corresponde con un número.", cadena[j]);
            exit(EXIT_FAILURE);
        }
	}

	int* vector = (int*) malloc (sizeof(int)*(len_cadena+1));
	if (!vector){
        fprintf(stderr, "ERROR: No se pudo reservar la memoria suficiente.");
        exit(EXIT_FAILURE);
	}

	const char sep[2] = " ";
	char *temp;
	temp = strtok(cadena, sep);

	int i = 0;

	while(temp != NULL){
		vector[i] = atoi(temp);
		i++;
		temp = strtok(NULL, sep);
	}
	return vector;
}

int obtener_largo_vector(int* vector){
    int i=0;

    while (vector[i]!='\0'){
        i ++;
    }

    return i;
}


void imprimir_vector(int* vector,int len_vector){
    int i=0;
    while (i<len_vector){
        printf("%i ", vector[i]);
        i++;
    }
    printf("\n");
}


void leer_archivo(const char* filename){

    int len_linea = 0;

    char c;

    FILE * archivo = fopen (filename,"r");
    if(!archivo){
        fprintf( stderr, "ERROR: No se pudo encontrar el archivo, verifique la ruta e intentelo nuevamente.");
        exit(EXIT_FAILURE);
        }

    while (c!=EOF){
        while (c!=EOF){
            c = fgetc(archivo);

            len_linea++;

            if (c=='\n'){
                fseek(archivo,-(len_linea)*sizeof(char),SEEK_CUR);
                break;
                }
        }

        if (c!=EOF && len_linea>1){
            char linea [len_linea];
            fgets(linea,len_linea,archivo);
            if (strcmp(linea,"\n") != 0){
                int* vector = convertir_a_vector(linea);
                int len_vector=obtener_largo_vector(vector);
                merge(vector, len_vector);
                imprimir_vector(vector,len_vector);

                free(vector);
            }
            else{
                printf("\n");
            }

            len_linea=0;

            fseek(archivo,sizeof(char),SEEK_CUR);
        }
    }

    fclose(archivo);
}

int main(int argc, char *argv[]){

	if(strcmp(argv[1],"-V") == 0){
		printf("versión TP1 Organización de computadoras\n");
		return 0;
	}
	if(strcmp(argv[1],"-h") == 0){
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
		return 0;
	}
	if(strcmp(argv[1],"-i") == 0){
		leer_archivo(argv[2]);
	}

	return 0;
}
