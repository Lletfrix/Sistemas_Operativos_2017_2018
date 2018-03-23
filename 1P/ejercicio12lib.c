/**
 * @brief Funciones comunes al ejercicio 12
 *
 * Este fichero contiene las implementaciones de las funciones comunes al ejercicio 12.
 * @file ejercicio12lib.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
 #include "ejercicio12lib.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>

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
