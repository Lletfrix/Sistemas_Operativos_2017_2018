#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX_HIJOS 4

void main(){
    int i, id;
    for(i = 0; i < MAX_HIJOS; i++){
        id = fork();
        if(id < 0){
            printf("Error al usar fork: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if(!id){
            printf("Soy el proceso hijo %d\n", getpid());
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
