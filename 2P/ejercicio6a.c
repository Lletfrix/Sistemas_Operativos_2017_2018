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
int main (void){
    sigset_t set, unset, oset;
    int pid, counter;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGALRM);
    sigaddset(&unset, SIGALRM);
    sigaddset(&unset, SIGUSR1);
    pid = fork();
    if (pid == 0){
        if(alarm(SECS)){
            printf("Error al crear la alarma: %s", strerror(errno));
        }
        while(1){
            if(sigprocmask(SIG_BLOCK, &set, &oset)){
                printf("Error al aplicar la máscara: %s", strerror(errno));
            }
            for (counter = 0; counter < NUM_PROC; counter++){
                printf("%d\n", counter);
                sleep(1);
            }
            if(sigprocmask(SIG_UNBLOCK, &unset, &oset)){
                printf("Error al aplicar la máscara: %s", strerror(errno));
            }
        sleep(3);
        }
    }
    while(wait(NULL)>0);
}
