#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int* convertir_a_vector(char* cadena){
    int* vector = malloc (sizeof(int));
    int len_vector = 0;

    char* palabra = strtok(cadena, " ");

    while( palabra != NULL ) {

      vector[len_vector] = atoi(palabra);
      palabra = strtok(NULL, " ");
      len_vector ++;
      vector= realloc(vector,len_vector * sizeof(int));
    }

    return vector;
}

int main(int argc, char *argv[]){
    int* vector= convertir_a_vector("1 2 3 4");
    for (int i=0;i<=3;i++){
        printf("%i\n",vector[i]);
    }

    for (int i=0;i<=3;i++){
        free(&vector[i]);
    }
    return 0;
}







