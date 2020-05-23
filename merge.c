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
            fprintf( stderr, "ERROR: Uno o más datos del archivo no son válidos, por ejemeplo el simbolo %c no se corresponde con un número.", string[j]);
            exit(EXIT_FAILURE);
        }
	}
	int* vector = malloc (sizeof(int)*(len_string));
	if (!vector){
        fprintf(stderr, "ERROR: No se pudo reservar la memoria suficiente.");
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
        i++;
    }
    fputs("\n",dfile);
}


void process_line(char* line,FILE* dfile){
    int* len = malloc(sizeof(int));
    int* vector = to_vector(line,len);
    if (*len>0){
    //merge(vector, len_vector);
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
        fprintf( stderr, "ERROR: No se puedo reservar memoria suficente para la lectura de la linea");
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




void process_file(char* filename,char* destination){
    FILE* dfile=NULL;
    if (strcmp(destination,"Unspecified") != 0){
        dfile= fopen(destination,"w+");
        if (!dfile){
            fprintf( stderr, "ERROR: no se ha podido crear el archivo en la ruta especificada");
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
        fprintf( stderr, "ERROR: ingrese la ruta del archivo de salida");
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
            fprintf( stderr, "ERROR: ingrese la ruta del archivo de entrada");
            exit(EXIT_FAILURE);
         }
         if (!argv[3]){
            process_file(argv[2],"Unspecified");
         }
		 else if(strcmp(argv[3],"-o") == 0){
            if (!argv[4]){
                fprintf( stderr, "ERROR: ingrese la ruta del archivo de salida");
                exit(EXIT_FAILURE);
            }
            process_file(argv[2],argv[4]);
		}

	}

	else{
        fprintf( stderr, "ERROR: uno o más parámetros son incorrectos");
        exit(EXIT_FAILURE);

	}

	return 0;
}
