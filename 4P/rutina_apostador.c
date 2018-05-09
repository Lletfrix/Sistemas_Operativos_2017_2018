#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#include "mylib.h"
#include "apostador.h"
#include "semaforos.h"
#include "sim_carr_lib.h"
#include "rutina_apostador.h"
#include "time.h"

volatile bool running_apostador = true;
Apostador *apostadores;
void _apos_handler(int sig);

void proc_apostador(int id){
    char name[MAX_APOS_NAME];
    struct msgapues mensaje;
    int shmid_apos = shmget(ftok(PATH, KEY_APOS_SHM), n_apos* sizeof(Apostador), SHM_W|SHM_R);
    int msgqid, semid_gen;
    double apuesta;
    void _apos_handler();
    sigset_t mask;

    apostadores = shmat(shmid_apos, NULL, 0);

    msgqid = msgget(ftok(PATH, KEY_APUES_Q), 0);

    sprintf(name, "Apostador-%d", id+1);
    apos_init(&apostadores[id], name, din);
    strcpy(mensaje.nombre, name);
    mensaje.mtype = id+1;

    srand(getpid());

    /* Establece la máscara de señales */
    sigfillset(&mask);
    sigdelset(&mask, SIGSTART);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    signal(SIGSTART, _apos_handler);

    crear_semaforo(ftok(PATH, KEY_GEN_SEM), num_proc, &semid_gen);
    down_semaforo(semid_gen, id + n_cab , 0);

    while(running_apostador){
        mensaje.caballo = randNum(0, n_cab);
        apuesta = randNum(0, din/TIEMPO_PRE_CARR);
        mensaje.cantidad = apuesta;
        if(-1 == msgsnd(msgqid, &mensaje, sizeof(struct msgapues) - sizeof(long), 0)){
            if(errno != EINTR){
                perror("Couldn't send message");
            }
        }
        apos_incr_din_rest(&apostadores[id], -1*apuesta);
        sleep(1);
    }
}

void _apos_handler(int sig){
    switch (sig) {
        case SIGSTART:
            running_apostador = false;
            break;
        default:
            return;
    }
}
