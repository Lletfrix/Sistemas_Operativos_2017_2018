#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdbool.h>
#include <time.h>

#include "sim_carr_lib.h"
#include "mylib.h"
#include "semaforos.h"
#include "rutina_tirada.h"

void _tirada_handler(int sig);

volatile bool running_tirada = true;

void proc_tirada(int id, int *pipe){
    sigset_t set, oldset;
    char tirada_type;
    unsigned short tirada;
    struct msgtir mensaje = {0};
    int msgqid, semid_gen;

    srand(getpid());

    close(pipe[WRITE]);

    signal(SIGTHROW, _tirada_handler);
    signal(SIGINT, _tirada_handler);
    signal(SIGSTART, _tirada_handler);

    sigemptyset(&set);
    sigaddset_var(&set, SIGTHROW, SIGINT, SIGSTART,-1);
    sigprocmask(SIG_BLOCK, &set, &oldset);

    crear_semaforo(ftok(PATH, KEY_GEN_SEM), num_proc, &semid_gen);
    down_semaforo(semid_gen, id, 0);

    sigsuspend(&oldset);
    while(running_tirada){
        tirada = 0;
        sigsuspend(&oldset);
        read(pipe[READ], &tirada_type, sizeof(char));
        switch (tirada_type) {
            case REMONTAR:
                tirada += (unsigned short) randNum(1, 7);
                tirada += (unsigned short) randNum(1, 7);
            break;
            case GANADORA:
                tirada += (unsigned short) randNum(1, 8);
            break;
            case NORMAL:
                tirada += (unsigned short) randNum(1, 7);
            break;
            default:
                perror("Switch default case in file rutina_tirada.c");
        }
        msgqid = msgget(ftok(PATH, KEY_TIR_Q), 0);
        mensaje.mtype = id+1;
        mensaje.tirada = tirada;
        if(msgsnd(msgqid, &mensaje, sizeof(struct msgtir) - sizeof(long), 0) == -1){
            perror("Couldn't send");
        }
    }
    exit(EXIT_SUCCESS);
}

void _tirada_handler(int sig){
    switch (sig) {
        case SIGTHROW:
        case SIGSTART:
            return;
        case SIGINT:
            running_tirada = false;
        default:
            return;
    }
}
