/**
 * @brief Ejercicio 2
 *
 * Este fichero contiene el código fuente del ejercicio 2 de la entrega.
 * @file ejercicio2.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 06-04-2018
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX_HIJOS 4 /*!< Numero maximo de hijos */

void main(){
    int i, id;
    for(i = 0; i < MAX_HIJOS; i++){
        id = fork();
        if(id < 0){
            printf("Error al usar fork: %s.", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if(!id){
            printf("Soy el proceso hijo %d.\n", getpid());
            fflush(stdout);
            sleep(30);
            printf("Soy el proceso hijo %d y ya me toca terminar.\n", getpid());
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }
        else{
            sleep(5);
            if(kill(id, SIGTERM)){
                printf("Error en kill: %s", strerror(errno));
            }
        }
    }
    exit(EXIT_SUCCESS);
}
