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
#include <unistd.h>

#include "rutina_gestor.h"
#include "sim_carr_lib.h"
#include "apostador.h"
#include "caballo.h"
#include "apuesta.h"
#include "semaforos.h"

struct _ventanilla{
    Caballo **caballos;
    Apostador **apostadores;
    int ventanilla;
    int n_cab;
    int msgqid;
};

void *_rutina_ventanilla(void *data);

volatile bool end = false;;

void _gestor_handler(int sig);

void proc_gestor(int n_vent, int n_cab, int n_apos){
    Caballo **caballos;
    Apostador **apostadores;
    int cab_shmid, apos_shmid, msgqid, i, caballo_mutex;
    int active[] = {0,1,2,3,4,5,6,7,8,9};
    pthread_t *threads, *mockthread;
    struct _ventanilla *e_ventanilla;
    void _gestor_handler();


    signal(SIGABRT, _gestor_handler);
    signal(SIGINT, _gestor_handler);
    signal(SIGSTART, _gestor_handler);
    /* Inicia el valor de apuesta de cada caballo */
    cab_shmid = shmget(ftok(PATH, KEY_CAB_SHM), n_cab * cab_sizeof(), 0);
    caballos = shmat(cab_shmid, NULL, 0);
    for (i = 0; i < n_cab; ++i){
        cab_incr_apostado(caballos[i], 1.0 - cab_get_apostado(caballos[i]));
        cab_set_cot(caballos[i], apuesta_total/cab_get_apostado(caballos[i]));
    }

    crear_semaforo(ftok(PATH, KEY_CAB_SEM), n_cab, &caballo_mutex);
    up_multiple_semaforo(caballo_mutex, n_cab, 0, active);
    /* El dinero a pagar de cada apostador ya se inicializa cuando se ejecuta la inicializacion en rutina_apostador */
    apos_shmid = shmget(ftok(PATH, KEY_APOS_SHM),n_apos * apos_sizeof(), 0);
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
        e_ventanilla->n_cab = n_cab;
        pthread_create(mockthread, NULL, _rutina_ventanilla, e_ventanilla);
    }
    //TODO: Aplicar mascara para que solo espere por SIGSTART o SIGABRT
    //sigsuspend();
    for(i = 0, mockthread = threads; i < n_vent; ++i, ++mockthread){
        //TODO: Ver si es mejor hacerlo con un while
        pthread_cancel(*mockthread);
    }
    exit(EXIT_SUCCESS);
}

void _gestor_handler(int sig){
    switch (sig) {
        case SIGSTART:
            return;
        case SIGABRT:

            exit(EXIT_SUCCESS);
        default:
            return;
    }
}

void *_rutina_ventanilla(void *data){
    struct msgapues mensaje;
    struct _ventanilla datos = * (struct _ventanilla *) data;
    Apuesta *apuesta;
    Caballo **caballos;
    Apostador **apostadores;
    int ventanilla;
    int n_cab;
    int msgqid;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    /* Copia el contenido de los parametros de entrada para poder liberar la memoria */
    caballos = datos.caballos;
    apostadores = datos.apostadores;
    ventanilla = datos.ventanilla;
    msgqid = datos.msgqid;
    n_cab = datos.n_cab;
    free(data);

    /* Recibe un mensaje */
    while(1){
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        msgrcv(msgqid, &mensaje, sizeof(struct msgapues) - sizeof(long), 0, 0);
        //TODO: Mirar que no se joda esto.
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        apuesta = apuesta_init(apuesta_new(), apostadores[mensaje.mtype], caballos[mensaje.caballo], ventanilla, mensaje.cantidad);
        apuesta_execute(apuesta, RUTA_FICHERO_APUESTAS, n_cab);
        apuesta_destroy(apuesta);
    }
    return NULL;
}
