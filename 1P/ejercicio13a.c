#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 512
#define NUM_THREADS 2

typedef struct datos_mult{
    int **matriz;
    int escalar;
    int size;
    int id;
} datos_mult_t;

void libera_matriz (int **matriz,unsigned int filas){
    int i;
    if(!matriz) return;

    for( i=0; i<filas; i++ ){
        free(matriz[i]);
    }
    free(matriz);
}

int **reserva_matriz(int size){
    int **m1, i;
    m1 = (int **) calloc(size, sizeof(int *));
    if(!m1) {
        return NULL;
    }
    for (i=0;i<size;i++) {
        m1[i] = (int *) calloc(size, sizeof(int));
        if (!m1[i]) {
            libera_matriz(m1, i);
            return NULL;
        }
    }
    return m1;
}

int **rellena_matriz (int** m1, char *buffer, int size){
    int i, j;
    char *token;
    token = strtok(buffer, " ");
    i = j = 0;
    while (token) {
        m1[i][j] = atoi(token);
        token = strtok(NULL, " ");
        ++j;
        if (!(j%size)){
            ++i;
            j = 0;
        }
    }
    return m1;
}

void *mult_matr_esc (void *data){
    int i, j;
    char buffer[BUFF_SIZE], minibuffer[BUFF_SIZE];
    datos_mult_t *data_struct = (datos_mult_t *) data;
    for (i = 0; i < data_struct->size; ++i) {
        for (j = 0; j < data_struct->size; ++j) {
            data_struct->matriz[i][j] *= data_struct->escalar;
            sprintf(minibuffer, " %d", data_struct->matriz[i][j]);
            strcat(buffer, minibuffer);
        }
        printf("Hilo %d multiplicando fila %d resultado%s\n", data_struct->id, i, buffer);
        sprintf(buffer, "");
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    int size, n1, n2, **m1, **m2, i, j;
    char buffer[BUFF_SIZE], *token;
    pthread_t threads[NUM_THREADS];
    datos_mult_t *datos1, *datos2;
    printf("Introduzca dimension de la matriz cuadrada:\n");
    size = atoi(fgets(buffer, BUFF_SIZE, stdin));
    if (size > 5) {
        printf("Introduzca una dimension no superior a 5\n");
    }
    printf("Introduzca multiplicador 1:\n");
    n1 = atoi(fgets(buffer, BUFF_SIZE, stdin));
    printf("Introduzca multiplicador 2:\n");
    n2 = atoi(fgets(buffer, BUFF_SIZE, stdin));

    m1 = reserva_matriz(size);
    if(!m1){
        exit(EXIT_FAILURE);
    }
    printf("Introduzca la matriz 1:\n");

    fgets(buffer, BUFF_SIZE, stdin);
    m1 = rellena_matriz(m1, buffer, size);

    m2 = reserva_matriz(size);
    if(!m2){
        libera_matriz(m1, size);
        exit(EXIT_FAILURE);
    }
    printf("Introduzca la matriz 2:\n");
    fgets(buffer, BUFF_SIZE, stdin);
    m2 = rellena_matriz(m2, buffer, size);

    datos1 = calloc(1, sizeof(datos_mult_t));
    if(!datos1){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        exit(EXIT_FAILURE);
    }
    datos1->matriz = m1;
    datos1->escalar = n1;
    datos1->size = size;
    datos1->id = 1;

    datos2 = calloc(1, sizeof(datos_mult_t));
    if(!datos2){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        free(datos1);
        exit(EXIT_FAILURE);
    }
    datos2->matriz = m2;
    datos2->escalar = n2;
    datos2->size = size;
    datos2->id = 2;

    pthread_create(&threads[0], NULL, mult_matr_esc, datos1);
    pthread_create(&threads[1], NULL, mult_matr_esc, datos2);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    exit (0);
}
