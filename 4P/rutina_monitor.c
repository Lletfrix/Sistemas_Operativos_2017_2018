#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

#include "rutina_apostador.h"
#include "sim_carr_lib.h"
#include "apostador.h"
#include "caballo.h"
#include "apuesta.h"

void _monitor_handler(int sig);
void _monitor_pre_carrera();
void _monitor_carrera();
void _monitor_post_carrera();

volatile bool fin_pre_carr = false;
volatile bool fin_carr = false;
volatile bool fin_post_carr = false;

void proc_monitor(int n_cab, int n_apos){
    Caballo **caballos;
    Apostador **apostadores;
    caballos = shmat(shmget(ftok(PATH, KEY_CAB_SHM), n_cab * cab_sizeof(), 0), NULL, 0);
    apostadores = shmat(shmget(ftok(PATH, KEY_APOS_SHM), n_apos * apos_sizeof(), 0), NULL, 0);
    signal(SIGSTART, _monitor_handler);
    signal(SIGINT, _monitor_handler);
    signal(SIGABRT, _monitor_handler);
    _monitor_pre_carrera(n_cab, caballos);
    _monitor_carrera();
    _monitor_post_carrera();
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
            fin_post_carr = true;
            return;
        default:
            return;
    }
}

void _monitor_pre_carrera(int n_cab, Caballo **caballos){
    int i = 30, j;
    while(1){
        printf("Quedan: %d s.\n", i);
        printf("La cotizacion de cada caballo actualmente es:\n");
        for (j = 0; j < n_cab; ++j) {
            //TODO: Control mutex de caballos, deberia hacer un down multilpe?
            printf("\tCaballo: %u - Cotizacion %f\n", cab_get_id(caballos[i])+1, cab_get_cot(caballos[i]));
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
    while(1){
        //TODO: Baja el monitor
            for (i = 0; i < n_cab; ++i) {
                printf("\tCaballo: %u - Posicion %u - Ultima tirada %d\n", cab_get_id(caballos[i])+1, cab_get_pos(caballos[i]), cab_get_last_tir(caballos[i]));
            }
        //TODO: Sube el principal
        }
        if(fin_carr){
            return;
        }
    }
}
