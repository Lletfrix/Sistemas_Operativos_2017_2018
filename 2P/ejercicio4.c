#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define NUM_ITER 10

void main(){
    int len, i, num, pid, last_pid;
    if(argc != 2){
        printf("Un único parámetro de entrada: el máximo número de hijos\n");
        exit(EXIT_FAILURE);
    }
    len = strlen(argv[1]);
    for(i = 0; i < len; i++){
        if(!isdigit(argv[1][i]){
            printf("El argumento pasado no es un número entero positivo\n");
            exit(EXIT_FAILURE);
        }
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
                kill(last_pid, SIGKILL);
            }
            j = 0;
            while(1){
                printf("Soy %d y estoy trabajando\n");
                sleep(1);
                j++;
                if(j == 10){
                    kill(getppid(), SIGCLD);
                }
            }
        }
        else{
            last_pid = pid;
            pause();
        }
    }
}
