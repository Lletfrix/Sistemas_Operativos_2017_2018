#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdbool.h>

#include "sim_carr_lib.h"
#include "mylib.h"
#include "rutina_tirada.h"

void _tirada_handler(int sig);

volatile bool running_tirada = true;

void proc_tirada(int id, int **pipe){
    sigset_t set, oldset;
    char tirada_type;
    unsigned short tirada;
    struct msgtir mensaje;
    int msgqid;

    close(pipe[id][WRITE]);
    signal(SIGTHROW, _tirada_handler);
    signal(SIGINT, _tirada_handler);
    signal(SIGABRT, _tirada_handler);
    signal(SIGSTART, _tirada_handler);
    sigemptyset(&set);
    sigaddset_var(&set, SIGTHROW, SIGINT, SIGABRT, SIGSTART,-1);
    sigprocmask(SIG_BLOCK, &set, &oldset);
    sigsuspend(&oldset);
    while(running_tirada){
        tirada = 0;
        sigsuspend(&oldset);
        read(pipe[id][READ], &tirada_type, sizeof(char));
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
        msgqid = msgget(ftok(PATH, KEY_CAB_SHM), 0);
        mensaje.mtype = id;
        mensaje.tirada = tirada;
        msgsnd(msgqid, &mensaje, sizeof(unsigned short), 0);
    }
    exit(EXIT_SUCCESS);
}

void _tirada_handler(int sig){
    switch (sig) {
        case SIGABRT:
        //TODO: Check if free resources is needed
            exit(EXIT_SUCCESS);
        case SIGTHROW:
        case SIGSTART:
            return;
        case SIGINT:
            running_tirada = false;
        default:
            return;
    }
}
