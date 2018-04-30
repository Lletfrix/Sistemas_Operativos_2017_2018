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

#define NUM_ARGS 5
#define MAX_APOS 20
#define KEY 1300
#define PATH "/bin/bash"
#define APOS_TYPE 1

void usage();
void init_log();

struct message{
    long mtype; /*!< Tipo de mensaje*/
    char nombre[MAX_APOS]; /*!< Información del mensaje*/
    int caballo;
    double cantidad;
};

static int qid;

int main(int argc, char* argv[]) {
    int i, ncab, longitud, napos, nvent, din, pid, n;
    int* caballos;
    if(argc != NUM_ARGS){
        usage();
        exit(EXIT_FAILURE);
    }

    if(n=strtof(argv[4], NULL) == 0){
        usage();
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < NUM_ARGS; i++){
        if(!aredigits(argv[i])){
            usage();
            exit(EXIT_FAILURE);
        }
    }

    ncab = atoi(argv[0]);
    longitud = atoi(argv[1]);
    napos = atoi(argv[2]);
    nvent = atoi(argv[3]);
    din = atoi(argv[4]);

    if((qid = msgget(ftok(PATH, KEY), IPC_CREAT|0600)) == -1){
        perror("Error al crear la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    if(ncab > 10 || napos > 100){
        usage();
        exit(EXIT_FAILURE);
    }

    init_log();

    caballos = malloc(ncab*sizeof(int));
    for(i = 0; i < ncab; i++){
        if(caballos[i] = fork() == -1){
            perror("Error en fork");
            while(wait(NULL) != -1);
            free(caballos);
            exit(EXIT_FAILURE);
        }
        if(!caballos[i]){
            caballo();
        }
    }

    if(pid = fork() == -1){
        perror("Error en fork");
        while(wait(NULL) != -1);
        free(caballos);
        exit(EXIT_FAILURE);
    }

    if(!pid){
        gestor_apuestas();
    }

    if(pid = fork() == -1){
        perror("Error en fork");
        while(wait(NULL) != -1);
        free(caballos);
        exit(EXIT_FAILURE);
    }

    if(!pid){
        apostador();
    }


    //syslog(LOG_NOTICE, "Session started!!");
    closelog();

    exit(EXIT_SUCCESS);
}

void usage(){
    printf("Usage is ./ProyectoFinal -ncaballos -longitudcarrera -napostadores -nventanillas -dineroinicial\n"\
    "ncaballos no puede ser mayor que 10 y napostadores no puede ser mayor que 100.");
}

void init_log(){
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("proyect_logger",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

void apostador(){
    int i, ncab;
    struct message m;
    char apos[MAX_APOS];
    for(i = 0; i < napos; i++){
        m.mtype = APOS_TYPE;
        sprintf(apos, "Apostador-%d", i+1);
        strcpy(m.nombre, apos);
        m.caballo = (int)randNum(1.0, ncab+0.005);
        m.cantidad = randNum(1, 5);
    }
}
