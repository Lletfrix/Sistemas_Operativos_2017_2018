/**
 * @brief Ejercicio 13b
 *
 * Este fichero contiene el código fuente del ejercicio 13b de la entrega.
 * @file ejercicio13b.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ejercicio13lib.h"

void *mult_matr_esc (void *data);

int main(int argc, char const *argv[]) {
    int size, n1, n2, **m1, **m2, i, j, fila_actual[NUM_THREADS];
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

    for  (i = 0; i < NUM_THREADS; i++) {
        fila_actual[i] = 0;
    }

    datos1 = datosMult_crear(m1, n1, size, 0);
    if(!datos1){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        exit(EXIT_FAILURE);
    }

    datos2 = datosMult_crear(m2, n2, size, 1);
    if(!datos2){
        libera_matriz(m1, size);
        libera_matriz(m2, size);
        free(datos1);
        exit(EXIT_FAILURE);
    }

    datos1->fila_actual[datos1->id] = &fila_actual[datos1->id];
    datos2->fila_actual[datos1->id] = &fila_actual[datos1->id];
    datos1->fila_actual[datos2->id] = &fila_actual[datos2->id];
    datos2->fila_actual[datos2->id] = &fila_actual[datos2->id];

    if (pthread_create(&threads[0], NULL, mult_matr_esc, datos1)){
        libera_matrix(m1, size);
        libera_matriz(m2, size);
        free(datos1);
        free(datos2);
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&threads[1], NULL, mult_matr_esc, datos2)){
        pthread_join(threads[0], NULL);
        libera_matrix(m1, size);
        libera_matriz(m2, size);
        free(datos1);
        free(datos2);
        exit(EXIT_FAILURE);
    }
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
* Además, imprime el estado del hilo contrario con cada multiplicacion de fila.
* @param data Estructura datosMult que almacena la informacion para ejecutar el hilo.
* @return NULL
*/
void *mult_matr_esc (void *data){
    int i, j, other_id;
    char buffer[BUFF_SIZE], minibuffer[BUFF_SIZE];
    datosMult *data_struct = (datosMult *) data;
    for (i = 0; i < data_struct->size; ++i) {
        (*data_struct->fila_actual[data_struct->id])++;
        sprintf(buffer, "Hilo %d multiplicando fila %d resultado ", data_struct->id+1, i);
        for (j = 0; j < data_struct->size; ++j) {
            data_struct->matriz[i][j] *= data_struct->escalar;
            sprintf(minibuffer, " %d", data_struct->matriz[i][j]);
            strcat(buffer, minibuffer);
        }
        other_id = (data_struct->id == 0) ? 1 : 0;
        if(*data_struct->fila_actual[other_id]){
            printf("%s. El hilo %d va por la fila %d\n", buffer,  other_id+1, *data_struct->fila_actual[other_id]);
        }else{
            printf("%s. El hilo %d aún no ha comenzado\n", buffer, other_id+1);
        }
        fflush(stdin);
        sprintf(buffer, "");
        sleep(2);
    }
    return NULL;
}
