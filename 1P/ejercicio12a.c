#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SEC_TO_NSEC 1000000000
#define DOUBLE_SEC_TO_NSEC 1000000000.0

typedef struct estructura_hilo {
    char string[100];
    int n;
} estructura_hilo_t;

int es_primo(int num){
    int i=2;
    while(i<=floor(sqrt(num))){
         if(num%i==0)
             return 0;
         else
             i++;
    }

    return 1;
}

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
}

int main(int argc, char const *argv[]) {
    estructura_hilo_t *e_hilo;
    struct timespec init, final;
    int i;
    long long nsec;
    double sec;
    if (argc < 2){
        printf("Necesito un numero de primos a calcular.\n");
        exit(-1);
    }
    e_hilo = calloc(1, sizeof(estructura_hilo_t));
    e_hilo->n = atoi(argv[1]);
    clock_gettime(CLOCK_REALTIME, &init);
    for (i = 0; i < 100; ++i) {
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
