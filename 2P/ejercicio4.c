#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_ITER 10

void manejador_SIGUSR1(int sig){
    return;
}

bool aredigits(char *string){
    int i;
    for(i = 0; i < strlen(string); ++i){
        if (!isdigit(string[i])){
            return false;
        }
    }
    return true;
}

void main(int argc, char* argv[]){
    int len, i, j, num, pid, last_pid;
    void manejador_SIGUSR1();
    if(argc != 2){
        printf("Un único parámetro de entrada: el máximo número de hijos\n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){
        printf("Error al cambiar el manejador de SIGUSR1: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    len = strlen(argv[1]);
    if(!aredigits(argv[1])){
        printf("El argumento pasado no es un número entero positivo\n");
        exit(EXIT_FAILURE);
    }
    if(!(num = atoi(argv[1]))){
        printf("El argumento pasado es 0, necesitamos un número positivo\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < num; i++){
        if((pid = fork()) == -1){
            printf("Error en fork: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if(!pid){
            if(i != 0){
                kill(last_pid, SIGTERM);
            }
            j = 0;
            while(1){
                printf("Soy %d y estoy trabajando\n", getpid());
                sleep(1);
                j++;
                if(j == 10){
                    kill(getppid(), SIGUSR1);
                }
            }
        }
        else{
            last_pid = pid;
            pause();
        }
    }
    kill(last_pid, SIGTERM);
    while (wait(NULL) != -1);
    exit(EXIT_SUCCESS);
}
