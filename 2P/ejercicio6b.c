#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#define NUM_PROC 5
#define SECS 10

void manejador_SIGTERM(int sig){
    printf("Soy %d y he recibido la señal SIGTERM\n", getpid());
    exit(EXIT_SUCCESS);
}

int main (void){
    void manejador_SIGTERM();
    int pid, counter;
    pid = fork();
    if (pid == 0){
        if(signal(SIGTERM, manejador_SIGTERM) == SIG_ERR){
            printf("Error en signal: %s", strerror(errno));
        }
        while(1){
            for (counter = 0; counter < NUM_PROC; counter++){
                printf("%d\n", counter);
                sleep(1);
            }
            sleep(3);
        }
    }
    sleep(40);
    kill(pid, SIGTERM);
    while(wait(NULL)>0);
}
