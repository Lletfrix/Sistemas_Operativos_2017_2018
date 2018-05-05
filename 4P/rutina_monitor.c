#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "rutina_apostador.h"
#include "sim_carr_lib.h"
#include "apostador.h"
#include "caballo.h"
#include "apuesta.h"
#include "semaforos.h"

#define MAX_APOS_PRINT 10

void _monitor_handler(int sig);
void _monitor_pre_carrera(int , Caballo **);
void _monitor_carrera(int , Caballo **);
void _monitor_post_carrera(int , Caballo **, int , Apostador **);
void _monitor_fin();

volatile bool fin_pre_carr = false;
volatile bool fin_carr = false;

void proc_monitor(){
    int shmid_cab, shmid_apos, semid_gen;
    Caballo **caballos;
    Apostador **apostadores;
    shmid_cab = shmget(ftok(PATH, KEY_CAB_SHM), n_cab * sizeof(Caballo *), 0);
    caballos = shmat(shmid_cab, NULL, 0);
    shmid_apos = shmget(ftok(PATH, KEY_APOS_SHM), n_apos * sizeof(Apostador *), 0);
    apostadores = shmat(shmid_apos, NULL, 0);
    //printf("Monitor: caballos: %d - %p, apostadores: %d - %p\n", shmid_cab, caballos, shmid_apos, apostadores);
    signal(SIGSTART, _monitor_handler);
    signal(SIGINT, _monitor_handler);
    signal(SIGABRT, _monitor_handler);
    crear_semaforo(ftok(PATH, KEY_GEN_SEM), num_proc, &semid_gen);
    printf("esperando a que el proceso principal me levante... - soy %d\n", semid_gen);
    down_semaforo(semid_gen, n_cab + n_apos, 0);
    _monitor_pre_carrera(n_cab, caballos);
    _monitor_carrera(n_cab, caballos);
    _monitor_post_carrera(n_cab, caballos, n_apos, apostadores);
    _monitor_fin();
    exit(EXIT_FAILURE);
}

void _monitor_handler(int sig){
    switch (sig) {
        case SIGSTART:
            fin_pre_carr = true;
            return;
        case SIGINT:
            fin_carr = true;
            return;
        case SIGABRT:
            //TODO:
            //fin_post_carr = true;
            return;
        default:
            return;
    }
}

void _monitor_pre_carrera(int n_cab, Caballo **caballos){
    int i = 30, j, caballo_mutex;
    int active[] = {0,1,2,3,4,5,6,7,8,9};
    while(1){
        printf("Quedan: %d s.\n", i);
        printf("La cotizacion de cada caballo actualmente es:\n");
        for (j = 0; j < n_cab; ++j) {
            //TODO: Control mutex de caballos, deberia hacer un down multilpe?
            crear_semaforo(ftok(PATH, KEY_CAB_SEM), n_cab, &caballo_mutex);
            down_multiple_semaforo(caballo_mutex, n_cab, 0, active);
            printf("\tCaballo: %u - Cotizacion %f\n", cab_get_id(caballos[j])+1, cab_get_cot(caballos[j]));
            up_multiple_semaforo(caballo_mutex, n_cab, 0, active);
        }
        sleep(1);
        if(i){
            i--;
        }
        if(fin_pre_carr){
            return;
        }
    }
}

void _monitor_carrera(int n_cab, Caballo **caballos){
    int i;
    int semid_mon, semid_turno;
    crear_semaforo(ftok(PATH, KEY_MON_SEM), 1, &semid_mon);
    crear_semaforo(ftok(PATH, KEY_TUR_SEM), 1, &semid_turno);
    while(1){
        down_semaforo(semid_mon, 0 , 0);
            for (i = 0; i < n_cab; ++i) {
                printf("\tCaballo: %u - Posicion %u - Ultima tirada %d\n", cab_get_id(caballos[i])+1, cab_get_pos(caballos[i]), cab_get_last_tir(caballos[i]));
            }
        up_semaforo(semid_turno, 0, 0);
        if(fin_carr){
            return;
        }
    }
}

void _monitor_post_carrera(int n_cab, Caballo **caballos, int n_apos, Apostador **apostadores){
    int i, max_pos = 0, aux_pos, lim_aux = MAX_APOS_PRINT;

    for (i = 0; i < n_cab; ++i) {
        if(max_pos < (aux_pos=cab_get_pos(caballos[i]))){
            max_pos = aux_pos;
        }
    }

    for (i = 0; i < n_cab; ++i) {
        if(max_pos == cab_get_pos(caballos[i])){
            printf("El caballo %u es ganador.\n", cab_get_pos(caballos[i]));
        }
    }

    qsort(apostadores, n_apos, sizeof(Apostador *), &apos_cmp_ben);
    if(n_apos < lim_aux){
        lim_aux = n_apos;
    }
    for(i = 0; i < lim_aux; ++i){
        printf("%d) %s: %f\n", i+1, apos_get_name(apostadores[i]), apos_get_ben(apostadores[i]));
    }

    return;
}

void _monitor_fin(){
    int fd;
    char c;
    fd = open(RUTA_FICHERO_APUESTAS,O_RDONLY);
    while(read(fd, &c, sizeof(char))){
        write(STDOUT_FILENO, &c, sizeof(char));
    }
    return;
}
