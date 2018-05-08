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
#include "semaforos.h"
#include "rutina_tirada.h"

void _tirada_handler(int sig);

volatile bool running_tirada = true;

void proc_tirada(int id, int **pipe){
    sigset_t set, oldset;
    char tirada_type;
    unsigned short tirada;
    struct msgtir mensaje;
    int msgqid, semid_gen;

    close(pipe[id][WRITE]);

    signal(SIGTHROW, _tirada_handler);
    signal(SIGINT, _tirada_handler);
    signal(SIGABRT, _tirada_handler);
    signal(SIGSTART, _tirada_handler);

    sigemptyset(&set);
    sigaddset_var(&set, SIGTHROW, SIGINT, SIGABRT, SIGSTART,-1);
    sigprocmask(SIG_BLOCK, &set, &oldset);

    crear_semaforo(ftok(PATH, KEY_GEN_SEM), num_proc, &semid_gen);
    down_semaforo(semid_gen, id, 0);

    printf("Voy a esperar por la señal SIGSTART\n");
    sigsuspend(&oldset);
    printf("Deje de esperar por SIGSTART\n");
    while(running_tirada){
        tirada = 0;
        printf("Voy a esperar por SIGTHROW\n");
        sigsuspend(&oldset);
        printf("Deje de esperar por SIGTHROW\n");
        read(pipe[id][READ], &tirada_type, sizeof(char));
        printf("Lei la pipe\n");
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
        printf("Tirada:%d\n", tirada);
        msgqid = msgget(ftok(PATH, KEY_TIR_Q), 0);
        mensaje.mtype = id+1;
        mensaje.tirada = tirada;
        printf("msgqid: %d, mensaj.mtype: %ld, tirada: %d\n",msgqid,mensaje.mtype, mensaje.tirada );
        if(msgsnd(msgqid, &mensaje, sizeof(struct msgtir) - sizeof(long), 0) == -1){
            perror("Couldn't send");
        }
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
