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

int main(int argc, char *argv[]){
	char cadena[7] = "1 2 3 4";
	int* vector = convertir_a_vector(cadena);
	for (int i=0;i<=3;i++){
		printf("%i\n",vector[i]);
	}
	free(vector);
	return 0;
}