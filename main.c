#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int* convertir_a_vector(char* cadena){
	int len_cadena = 0;
	for(int j = 0; (cadena[j] != '\0');j++){
		if(cadena[j] == ' ')len_cadena++;
	}
	int* vector = (int*) malloc (sizeof(int)*(len_cadena+1));
	const char sep[2] = " ";
	char *temp;
	//obtengo primer temp
	temp = strtok(cadena, sep);

	int i = 0;

	while(temp != NULL){
		vector[i] = atoi(temp);
		i++;
		temp = strtok(NULL, sep);
	}
	return vector;
}

void leer_archivo(const char* filename){

    int len_linea = 0;

    char c;

    FILE * archivo = fopen (filename,"r");

    while (c!=EOF){
        while (c!=EOF){
            c = fgetc(archivo);

            if (c=='\n'){
                fseek(archivo,-(len_linea)*sizeof(char),SEEK_CUR);
                break;
                }

            len_linea++;
        }

        if (c!=EOF){
            char linea [len_linea];
            fgets(linea,len_linea,archivo);

            int* vector = convertir_a_vector(linea);
            for (int i=0;i<=3;i++){
                printf("%i\n",vector[i]);
            }
            free(vector);

            len_linea=0;
            fseek(archivo,sizeof(char),SEEK_CUR);
        }
    }

    printf("sali");
    fclose(archivo);
}

int main(int argc, char *argv[]){
	/*char cadena[] = "1 2 3 4";
	int* vector = convertir_a_vector(cadena);
	for (int i=0;i<=3;i++){
		printf("%i\n",vector[i]);
	}
	free(vector);*/

	leer_archivo("prueba.txt");
	return 0;
}
