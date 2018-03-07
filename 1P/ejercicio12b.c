#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 100 /*!< Constante que define el numero de hilos a ejecutar*/
#define SEC_TO_NSEC 1000000000 /*!< Constante de cambio entre segundos y nanosegundos */
#define DOUBLE_SEC_TO_NSEC 1000000000.0 /*!< Constante de cambio entre segundos y nanosegundos como float*/

/**
 * @brief Datos del hilo
 *
 * Esta estructura define los datos que usará el hilo, una string de
 * 100 caracteres y un entero n.
 */
typedef struct estructura_hilo {
    char string[100];
    int n;
} estructura_hilo_t;

/**
 * @brief Comprueba si un numero es primo
 *
 * es_primo() comprueba si un numero dado es primo
 * @param num entero al que comprobar si es primo o no
 * @return 1 si el número es primo, 0 en caso contrario
 */
int es_primo(unsigned int num){
    int i=2;
    while(i<=floor(sqrt(num))){
         if(num%i==0)
             return 0;
         else
             i++;
    }
    return 1;
}

/**
 * @brief Calcula un numero determinado de primos
 *
 * calcular_primos() calcula un numero de primos que recibe por la
 * entrada estructura_hilo, que recibe como puntero a void
 * por el argumento de entrada.
 * @param void_data estructura_hilo casteada a (void *)
 * @return NULL
 */
void *calcular_primos(void *void_data){
    int *primos, i = 0, j = 2, num_primos;
    estructura_hilo_t *data = (estructura_hilo_t *) void_data;
    num_primos = data->n;
    primos = calloc(num_primos, sizeof(int));
    while(i != num_primos-1){
        if(es_primo(j)){
            primos[i] = j;
            ++i;
        }
        ++j;
    }
    free(primos);
    return NULL;
}

int main(int argc, char const *argv[]) {
    estructura_hilo_t *e_hilo;
    struct timespec init, final;
    int i;
    pthread_t threads[NUM_THREADS];
    long long nsec;
    double sec;
    if (argc < 2){
        printf("Necesito un numero de primos a calcular.\n");
        exit(-1);
    }
    e_hilo = calloc(1, sizeof(estructura_hilo_t));
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
