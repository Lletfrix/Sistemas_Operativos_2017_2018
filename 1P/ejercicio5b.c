/**
 * @brief Ejercicio 5b
 *
 * Este fichero contiene el código fuente del ejercicio 5b de la entrega.
 * @file ejercicio5b.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROC 6
int main (void){
    int pid;
    int i;
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 != 0) {
            if ((pid=fork()) <0 ){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }else if (pid ==0){
                printf("HIJO %d \n", getpid());
                printf ("PADRE %d \n", getppid());
                break;
            }
        }
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}
