/**
 * @brief Funciones comunes al ejercicio 12
 *
 * Este fichero contiene las implementaciones de las funciones comunes al ejercicio 12.
 * @file ejercicio12lib.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */

#define NUM_PROCS 100 /*!< Constante que define el numero de procesos a ejecutar*/
#define NUM_THREADS 100 /*!< Constante que define el numero de hilos a ejecutar*/
#define SEC_TO_NSEC 1000000000 /*!< Constante de cambio entre segundos y nanosegundos */
#define DOUBLE_SEC_TO_NSEC 1000000000.0 /*!< Constante de cambio entre segundos y nanosegundos como float*/

typedef struct _estructuraHilo {
    char string[100];
    int n; /*!< Numero de primos a calcular */
};

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

void *calcular_primos(void *void_data){
    int *primos, i = 0, j = 2, num_primos;
    estructuraHilo *data = (estructuraHilo *) void_data;
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
