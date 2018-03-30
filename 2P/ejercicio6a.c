#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

#define NUM_PROC 5
#define SECS 40

void sigaddset_var(sigset_t *sigt, int sig, ...){
    va_list args;
    int i;

    va_start(args, sig);
    for (i = sig; i != -1; i = va_arg(args, int)) {
        sigaddset(sigt, i);
    }
    va_end(args);
    return;
}


int main (void){
    sigset_t set, unset, oset;
    int pid, counter;
    sigemptyset(&set);
    sigemptyset(&unset);
    sigaddset_var(&set, SIGUSR1, SIGUSR2, SIGALRM, -1);
    sigaddset_var(&unset, SIGALRM, SIGUSR1, -1);
    pid = fork();
    if (pid == 0){
        if(alarm(SECS)){
            printf("Error al crear la alarma: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        while(1){
            if(sigprocmask(SIG_BLOCK, &set, &oset)){
                printf("Error al aplicar la máscara: %s", strerror(errno));
                exit(EXIT_FAILURE);
            }
            for (counter = 0; counter < NUM_PROC; counter++){
                printf("%d\n", counter);
                sleep(1);
            }
            if(sigprocmask(SIG_UNBLOCK, &unset, &oset)){
                printf("Error al aplicar la máscara: %s", strerror(errno));
                exit(EXIT_FAILURE);
            }
        sleep(3);
        }
    }
    while(wait(NULL)!=-1);
}
