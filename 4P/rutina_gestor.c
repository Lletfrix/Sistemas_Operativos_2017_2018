#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>

#include "rutina_gestor.h"
#include "sim_carr_lib.h"
#include "apostador.h"
#include "caballo.h"
#include "apuesta.h"
#include "semaforos.h"

struct _ventanilla{
    Caballo *caballos;
    Apostador *apostadores;
    int ventanilla;
    int msgqid;
};

void *_rutina_ventanilla(void *data);

volatile bool end = false;

void _gestor_handler(int sig);

void proc_gestor(){
    Caballo *caballos;
    Apostador *apostadores;
    int cab_shmid, apos_shmid, msgqid, i, caballo_mutex, semid_gen;
    int active[] = {0,1,2,3,4,5,6,7,8,9};
    pthread_t *threads, *mockthread;
    struct _ventanilla *e_ventanilla;
    void _gestor_handler();
    sigset_t mask, set_start;

    signal(SIGSTART, _gestor_handler);
    signal(SIGINT, _gestor_handler);
    signal(SIGUSR2, _gestor_handler);

    /* Establece la máscara de señales */
    sigfillset(&mask);
    sigdelset(&mask, SIGSTART);
    sigdelset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&set_start);
    sigaddset(&set_start, SIGSTART);

    /* Inicia el valor de apuesta de cada caballo */
    cab_shmid = shmget(ftok(PATH, KEY_CAB_SHM), n_cab * sizeof(Caballo), 0);
    caballos = shmat(cab_shmid, NULL, 0);
    for (i = 0; i < n_cab; ++i){
        cab_incr_apostado(&caballos[i], 1.0 - cab_get_apostado(&caballos[i]));
        cab_set_cot(&caballos[i], 1.0);
    }

    crear_semaforo(ftok(PATH, KEY_GEN_SEM), num_proc, &semid_gen);
    down_semaforo(semid_gen, n_apos+n_cab+1, 0);

    /* El dinero a pagar de cada apostador ya se inicializa cuando se ejecuta la inicializacion en rutina_apostador */
    apos_shmid = shmget(ftok(PATH, KEY_APOS_SHM),n_apos * sizeof(Apostador), 0);
    apostadores = shmat(apos_shmid, NULL, 0);

    /* Obtiene la cola de mensajes */
    msgqid = msgget(ftok(PATH, KEY_APUES_Q), 0);

    threads = malloc(n_vent*sizeof(pthread_t));

    for(i = 0, mockthread = threads; i < n_vent; ++i, ++mockthread){
        //TODO: Error handling
        e_ventanilla = calloc(1, sizeof(struct _ventanilla));
        e_ventanilla->caballos = caballos;
        e_ventanilla->apostadores = apostadores;
        e_ventanilla->msgqid = msgqid;
        e_ventanilla->ventanilla = i;
        pthread_create(mockthread, NULL, _rutina_ventanilla, e_ventanilla);
    }

    sigsuspend(&set_start);
    for(i = 0, mockthread = threads; i < n_vent; ++i, ++mockthread){
        pthread_kill(*mockthread, SIGUSR2);
        pthread_join(*mockthread, NULL);
    }

    free(threads);

    exit(EXIT_SUCCESS);
}

void _gestor_handler(int sig){
    switch (sig) {
        case SIGSTART:
            return;
        case SIGINT:
            return;
        case SIGUSR2:
            end = true;
            break;
        default:
            return;
    }
}

void *_rutina_ventanilla(void *data){
    struct msgapues mensaje;
    struct _ventanilla datos = * (struct _ventanilla *) data;
    Apuesta *apuesta;
    Caballo *caballos;
    Apostador *apostadores;
    int ventanilla;
    int msgqid;

    /* Copia el contenido de los parametros de entrada para poder liberar la memoria */
    caballos = datos.caballos;
    apostadores = datos.apostadores;
    ventanilla = datos.ventanilla;
    msgqid = datos.msgqid;
    free(data);

    /* Recibe un mensaje */
    while(1){
        if(-1 == msgrcv(msgqid, &mensaje, sizeof(struct msgapues) - sizeof(long), 0, 0)){
            if(errno != EINTR){
                perror("Couldn't recieve message");
            }
        };
        apuesta = apuesta_init(apuesta_new(), &apostadores[mensaje.mtype-1], caballos, mensaje.caballo, ventanilla, mensaje.cantidad);
        apuesta_execute(apuesta, RUTA_FICHERO_APUESTAS);
        apuesta_destroy(apuesta);
        if(end){
            pthread_exit(NULL);
        }
    }
    return NULL;
}
