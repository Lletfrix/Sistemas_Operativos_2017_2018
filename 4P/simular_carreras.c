#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

#include "mylib.h"
#include "semaforos.h"
#include "sim_carr_lib.h"

#define NUM_ARGS 5
#define MAX_CAB 10
#define MAX_APOS 100
#define APOS_TYPE 1

void usage();
void init_log();
void _killall(int sig, pid_t monitor, pid_t gestor, Caballo **caballos, int n_cab, Apostador **apostadores, int n_apos);

int main(int argc, char* argv[]) {
    int i, n_cab, longitud, n_apos, n_vent, din, pid_aux, qid_apues, qid_tir, shmid_cab, shmid_apos, max_pos, min_pos, pos_aux;
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

    n_cab = atoi(argv[0]);
    longitud = atoi(argv[1]);
    n_apos = atoi(argv[2]);
    n_vent = atoi(argv[3]);
    din = atof(argv[4]);

    if(n_cab > MAX_CAB || n_apos > MAX_APOS){
        usage();
        exit(EXIT_FAILURE);
    }

    /* Reserva de IPCS */
    //TODO: Añadir semaforos
    //TODO: Añadir liberacion de recursos al control de errores
    if((qid_apues = msgget(ftok(PATH, KEY_APUES_Q), IPC_CREAT|0600)) == -1){
        perror("Error al crear la cola de mensajes APOSTADOR-GESTOR");
        exit(EXIT_FAILURE);
    }
    if(qid_tir = msgget(ftok(PATH, KEY_TIR_Q), IPC_CREAT|0600) == -1){
        perror("Error al crear la cola de mensajes CABALLO-MAIN");
        exit(EXIT_FAILURE);
    }
    if(shmid_cab = shmget(ftok(PATH, KEY_CAB_SHM), n_cab*cab_sizeof(), SHM_W|SHM_R|IPC_CREAT) == -1){
        perror("Error al crear la memoria compartida de los caballos");
        exit(EXIT_FAILURE);
    }
    if(shmid_apos = shmget(ftok(PATH, KEY_APOS_SHM), n_apos*apos_sizeof(), SHM_W|SHM_R|IPC_CREAT) == -1){
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
        proc_monitor();
        exit(EXIT_FAILURE);
    }
    /* Crea el gestor de apuestas */
    if((gestor = fork()) == -1){
        perror("Error en fork");
        //TODO: Añadir liberacion de recursos
        exit(EXIT_FAILURE);
    }
    if(!gestor){
        proc_gestor(n_vent);
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
            proc_apostador(i, din);
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
        cab_set_pid(caballo[i], pid_aux);
        cab_set_id(caballo[i], i+1);
    }

    /* Espera a que empiece la carrera */
    sleep(30);

    _killall(SIGSTART, monitor, gestor, caballos, n_cab, apostadores, n_apos);

    max_pos = 0;
    for (i = 0; i < n_cab; i++) {
        printf("%s\n", );
    }
    //TODO: Open pipe
    while(max_pos < longitud){
        min_pos = longitud;
        for(i = 0; i < n_cab; ++i){
            kill(SIGTHROW, cab_get_pid(caballos[i]));
        }
        /* Actualiza la posicion de cada caballo junto a la posicion maxima y minima */
        for(i = 0; i < n_cab; ++i){
            msgrcv(qid_tir, (struct msgbuf *) &mensaje_tirada, 0, 0);
            pos_aux = cab_get_pos(caballos[mensaje_tirada->mtype]) + mensaje_tirada->tirada;
            cab_set_pos(caballos[mensaje_tirada->mtype], pos_aux);
            if(pos_aux > max_pos){
                max_pos = pos_aux;
            }
            if(pos_aux < min_pos){
                min_pos = pos_aux;
            }
        }

        for (i = 0;  i < n_cab; ++i){
            if(cab_get_pos(caballos[i]) == min_pos){
                tirada_type = REMONTAR;
            }else if(cab_get_pos(caballos[i]) == max_pos){
                tirada_type = GANADORA;
            }else{
                tirada_type = NORMAL
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
    "n_caballos no puede ser mayor que 10 y n_apostadores no puede ser mayor que 100.");
}

void init_log(){
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("proyect_logger",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

/*void apostador(){
    int i, n_cab;
    struct message m;
    char apos[MAX_APOS_NAME];
    for(i = 0; i < n_apos; i++){
        m.mtype = APOS_TYPE;
        sprintf(apos, "Apostador-%d", i+1);
        strcpy(m.nombre, apos);
        m.caballo = (int)randNum(1.0, n_cab+0.005);
        m.cantidad = randNum(1, 5);
    }
}*/

void _killall(int sig, pid_t monitor, pid_t gestor, Caballo **caballos, int n_cab, Apostador **apostadores, int n_apos){
    kill(sig, monitor);
    kill(sig, gestor);
    for(i = 0; i < n_cab; ++i){
        kill(sig, cab_get_pid(caballos[i]));
    }
    for(i = 0; i < n_apos; ++i){
        kill(sig, apos_get_pid(apostadores[i]));
    }
}
