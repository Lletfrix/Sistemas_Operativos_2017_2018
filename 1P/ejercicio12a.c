/**
 * @brief Ejercicio 12a
 *
 * Este fichero contiene el código fuente del ejercicio 12a de la entrega.
 * @file ejercicio12a.c
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
#include "ejercicio12lib.h"

int main(int argc, char const *argv[]) {
    estructuraHilo *e_hilo;
    struct timespec init, final;
    int i;
    long long nsec;
    double sec;
    if (argc < 2){
        printf("Necesito un numero de primos a calcular.\n");
        exit(-1);
    }
    e_hilo = calloc(1, sizeof(estructuraHilo));
    e_hilo->n = atoi(argv[1]);
    clock_gettime(CLOCK_REALTIME, &init);
    for (i = 0; i < NUM_PROCS; ++i) {
        if(!fork()){
            calcular_primos(e_hilo);
            free(e_hilo);
            exit(0);
        }
    }
    while (wait(NULL) != -1);
    clock_gettime(CLOCK_REALTIME, &final);
    nsec = SEC_TO_NSEC*(final.tv_sec - init.tv_sec) + (final.tv_nsec - init.tv_nsec);
    sec = (double) nsec/DOUBLE_SEC_TO_NSEC;
    printf("La ejecución con procesos tarda: %lf segundos\n", sec);
    free(e_hilo);
    exit(0);
}
