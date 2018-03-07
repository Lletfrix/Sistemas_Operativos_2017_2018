/**
 * @brief Ejercicio 12b
 *
 * Este fichero contiene el código fuente del ejercicio 12b de la entrega.
 * @file ejercicio12b.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include "ejercicio12.h"

int main(int argc, char const *argv[]) {
    estructuraHilo *e_hilo;
    struct timespec init, final;
    int i;
    pthread_t threads[NUM_THREADS];
    long long nsec;
    double sec;
    if (argc < 2){
        printf("Necesito un numero de primos a calcular.\n");
        exit(-1);
    }
    e_hilo = calloc(1, sizeof(estructuraHilo));
    e_hilo->n = atoi(argv[1]);
    clock_gettime(CLOCK_REALTIME, &init);
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, calcular_primos, e_hilo);
    }
    for (i = 0; i < NUM_THREADS; ++i){
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &final);
    nsec = SEC_TO_NSEC*(final.tv_sec - init.tv_sec) + (final.tv_nsec - init.tv_nsec);
    sec = (double) nsec/DOUBLE_SEC_TO_NSEC;
    printf("La ejecución con hilos tarda: %lf segundos\n", sec);
    free(e_hilo);
    exit(0);
}
