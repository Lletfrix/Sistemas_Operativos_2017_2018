#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include "mylib.h"
#include "semaforos.h"
#include "sim_carr_lib.h"
#include "caballo.h"
#include "apostador.h"
#include "apuesta.h"
#include "rutina_monitor.h"
#include "rutina_gestor.h"
#include "rutina_tirada.h"
#include "rutina_apostador.h"

#define NUM_ARGS 6
#define MAX_CAB 10
#define MAX_APOS 100
#define APOS_TYPE 1

void usage();
void init_log();
void _killall(int sig, pid_t monitor, pid_t gestor, Caballo **caballos, int n_cab, Apostador **apostadores, int n_apos);

int main(int argc, char* argv[]) {
    int i, n_cab, longitud, n_apos, n_vent, din, pid_aux, max_pos, min_pos, pos_aux;
    int semid_mon, semid_turno, qid_apues, qid_tir, shmid_cab, shmid_apos;
    unsigned short sem_initial_val;
    int **fd;
    char tirada_type;
    Caballo **caballos;
    Apostador **apostadores;
    pid_t gestor, monitor;
    struct msgtir mensaje_tirada;

    /* Comprobacion de parametros de entrada*/
    if(argc != NUM_ARGS){
        usage();
        exit(EXIT_FAILURE);
    }

    for(i = 1; i < NUM_ARGS-1; i++){
        if(!aredigits(argv[i])){
            usage();
            exit(EXIT_FAILURE);
        }
    }

    if(!isfloat(argv[NUM_ARGS-1])){
        usage();
        exit(EXIT_FAILURE);
    }

    n_cab = atoi(argv[1]);
    longitud = atoi(argv[2]);
    n_apos = atoi(argv[3]);
    n_vent = atoi(argv[4]);
    din = atof(argv[5]);

    if(n_cab > MAX_CAB || n_apos > MAX_APOS){
        usage();
        exit(EXIT_FAILURE);
    }

    /* Reserva de IPCS */
    //TODO: Añadir semaforos
    sem_initial_val = 0;
    if(crear_semaforo(ftok(PATH, KEY_SEM_MON), 1, &semid_mon) == ERROR){
        perror("Error al crear el semaforo monitor");
        exit(EXIT_FAILURE);
    }
    sem_initial_val = 1;
    if(crear_semaforo(ftok(PATH, KEY_SEM_TUR), 1, &semid_turno) == ERROR){
        perror("Error al crear el semaforo turno");
        exit(EXIT_FAILURE);
    }
    if(inicializar_semaforo(semid_mon, &sem_initial_val) == ERROR){
        perror("Error al inicializar el semaforo monitor");
    }
    if(inicializar_semaforo(semid_mon, &sem_initial_val) == ERROR){
        perror("Error al inicializar el semaforo turno");
    }
    //TODO: Añadir liberacion de recursos al control de errores
    if((qid_apues = msgget(ftok(PATH, KEY_APUES_Q), IPC_CREAT|0600)) == -1){
        perror("Error al crear la cola de mensajes APOSTADOR-GESTOR");
        exit(EXIT_FAILURE);
    }
    if(qid_tir = msgget(ftok(PATH, KEY_TIR_Q), IPC_CREAT|0600) == -1){
        perror("Error al crear la cola de mensajes CABALLO-MAIN");
        exit(EXIT_FAILURE);
    }
    if(shmid_cab = shmget(ftok(PATH, KEY_CAB_SHM), n_cab*cab_sizeof(), SHM_W|SHM_R|IPC_CREAT|0600) == -1){
        perror("Error al crear la memoria compartida de los caballos");
        exit(EXIT_FAILURE);
    }
    if(shmid_apos = shmget(ftok(PATH, KEY_APOS_SHM), n_apos*apos_sizeof(), SHM_W|SHM_R|IPC_CREAT|0600) == -1){
        perror("Error al crear la memoria compartida de los apostadores");
        exit(EXIT_FAILURE);
    }

    caballos = shmat(shmid_cab, NULL, 0);
    apostadores = shmat(shmid_apos, NULL, 0);

    fd = calloc(n_cab, sizeof(int *));
    for (i = 0; i < n_cab; ++i) {
        fd[i] = calloc(2, sizeof(int));
    }

    for(i = 0; i < n_cab; ++i){
        if(pipe(fd[i]) == -1){
            perror("Error al crear una pipe");
            exit(EXIT_FAILURE);
        }
    }


    init_log();

    /* Creacion de procesos hijo */

    /* Crea el monitor */
    if((monitor = fork()) == -1){
        perror("Error en fork");
        //TODO: Añadir liberacion de recursos
        exit(EXIT_FAILURE);
    }
    if(!monitor){
        proc_monitor(n_cab, n_apos, semid_mon, semid_turno);
        exit(EXIT_FAILURE);
    }
    /* Crea el gestor de apuestas */
    if((gestor = fork()) == -1){
        perror("Error en fork");
        //TODO: Añadir liberacion de recursos
        exit(EXIT_FAILURE);
    }
    if(!gestor){
        proc_gestor(n_vent, n_cab, n_apos);
        exit(EXIT_FAILURE);
    }
    /* Crea los procesos apostadores */
    for(i = 0; i < n_apos; ++i){
        pid_aux = fork();
        if(pid_aux == -1){
            perror("Error en fork");
            //TODO: Añadir liberacion de recursos
            exit(EXIT_FAILURE);
        }
        if(!pid_aux){
            proc_apostador(i, din, n_cab, n_apos);
            exit(EXIT_FAILURE);
        }
        apos_set_pid(apostadores[i], pid_aux);
    }
    /* Crea los procesos de tirada */
    for(i = 0; i < n_cab; ++i){
        pid_aux = fork();
        if(pid_aux == -1){
            perror("Error en fork");
            //TODO: Añadir liberacion de recursos
            exit(EXIT_FAILURE);
        }
        if(!pid_aux){
            proc_tirada(i, fd);
            exit(EXIT_FAILURE);
        }
        cab_set_pid(caballos[i], pid_aux);
        cab_set_id(caballos[i], i);
    }

    /* Espera a que empiece la carrera */
    sleep(30);

    _killall(SIGSTART, monitor, gestor, caballos, n_cab, apostadores, n_apos);

    max_pos = 0;
    //TODO: Open pipe
    while(max_pos < longitud){
        min_pos = longitud;
        for(i = 0; i < n_cab; ++i){
            kill(SIGTHROW, cab_get_pid(caballos[i]));
        }
        /* Actualiza la posicion de cada caballo junto a la posicion maxima y minima */
        down_semaforo(semid_turno, 0, 0);
        for(i = 0; i < n_cab; ++i){
            msgrcv(qid_tir, (struct msgbuf *) &mensaje_tirada, sizeof(struct msgtir) - sizeof(long),0, 0);
            pos_aux = cab_get_pos(caballos[mensaje_tirada.mtype]) + mensaje_tirada.tirada;
            cab_set_last_tir(caballos[mensaje_tirada.mtype], mensaje_tirada.tirada);
            cab_set_pos(caballos[mensaje_tirada.mtype], pos_aux);
            if(pos_aux > max_pos){
                max_pos = pos_aux;
            }
            if(pos_aux < min_pos){
                min_pos = pos_aux;
            }
        }
        up_semaforo(semid_mon, 0, 0);
        for (i = 0;  i < n_cab; ++i){
            if(cab_get_pos(caballos[i]) == min_pos){
                tirada_type = REMONTAR;
            }else if(cab_get_pos(caballos[i]) == max_pos){
                tirada_type = GANADORA;
            }else{
                tirada_type = NORMAL;
            }
            write(fd[i][WRITE], &tirada_type, sizeof(char));
        }
    }
    /* Notifica del final de la carrera */
    _killall(SIGINT, monitor, gestor, caballos, n_cab, apostadores, n_apos);
    //TODO: Liberar recursos

    while(wait(NULL) != -1);

    closelog();
    exit(EXIT_SUCCESS);
}

void usage(){
    printf("Usage is ./simularCarrera <n_caballos> <longitudcarrera> <n_apostadores> <n_ventanillas> <dineroinicial>\n"\
    "n_caballos no puede ser mayor que 10 y n_apostadores no puede ser mayor que 100.\n");
}

void init_log(){
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("proyect_logger",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

void _killall(int sig, pid_t monitor, pid_t gestor, Caballo **caballos, int n_cab, Apostador **apostadores, int n_apos){
    int i;
    kill(sig, monitor);
    kill(sig, gestor);
    for(i = 0; i < n_cab; ++i){
        kill(sig, cab_get_pid(caballos[i]));
    }
    for(i = 0; i < n_apos; ++i){
        kill(sig, apos_get_pid(apostadores[i]));
    }
}
