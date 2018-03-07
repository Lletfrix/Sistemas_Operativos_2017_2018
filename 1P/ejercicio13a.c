/**
 * @brief Ejercicio 13a
 *
 * Este fichero contiene el código fuente del ejercicio 13a de la entrega.
 * @file ejercicio13a.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ejercicio13.h"

void *mult_matr_esc (void *data);


int main(int argc, char const *argv[]) {
    int size, n1, n2, **m1, **m2, i, j;
    char buffer[BUFF_SIZE], *token;
    pthread_t threads[NUM_THREADS];
    datosMult *datos1, *datos2;
    printf("Introduzca dimension de la matriz cuadrada:\n");
    size = atoi(fgets(buffer, BUFF_SIZE, stdin));
    if (size > 5) {
        printf("Introduzca una dimension no superior a 5\n");
        exit(-1);
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

    datos1 = datosMult_crear(m1, n1, size, 1);
    if(!datos1){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        exit(EXIT_FAILURE);
    }

    datos2 = datosMult_crear(m2, n2, size, 2);
    if(!datos2){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        free(datos1);
        exit(EXIT_FAILURE);
    }

    pthread_create(&threads[0], NULL, mult_matr_esc, datos1);
    pthread_create(&threads[1], NULL, mult_matr_esc, datos2);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    libera_matriz(m1, size);
    libera_matriz(m2, size);
    free(datos1);
    free(datos2);

    exit (0);
}

/**
* @brief Multiplica la matriz por el escalar
*
* Dada una matriz y un escalar los multiplica entre sí.
* @param data Estructura datosMult que almacena la informacion para ejecutar el hilo.
* @return NULL
*/
void *mult_matr_esc (void *data){
    int i, j;
    char buffer[BUFF_SIZE], minibuffer[BUFF_SIZE];
    datosMult *data_struct = (datosMult *) data;
    for (i = 0; i < data_struct->size; ++i) {
        for (j = 0; j < data_struct->size; ++j) {
            data_struct->matriz[i][j] *= data_struct->escalar;
            sprintf(minibuffer, " %d", data_struct->matriz[i][j]);
            strcat(buffer, minibuffer);
        }
        printf("Hilo %d multiplicando fila %d resultado%s\n", data_struct->id, i, buffer);
        sprintf(buffer, "");
        sleep(2);
    }
    return NULL;
}
