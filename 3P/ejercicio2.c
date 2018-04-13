#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdbool.h>

#include "mylib.h"

#define KEY 1300
#define PATH "/bin/bash"
#define MAX_NOMBRE 80

typedef struct info {
    char nombre[MAX_NOMBRE];
    int id;
} Info;

void handle_SIGUSR1(int sig);

void rutina_hijo();

void usage();

Info *buff = NULL;

int main(int argc, char const *argv[]) {
    int mem, n, i;
    pid_t pid;
    key_t key;

    if(argc != 2){
        usage();
        exit(EXIT_FAILURE);
    }

    if(!aredigits(argv[1])){
        usage();
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    if((key = ftok(PATH, KEY)) == -1){
        perror("Fallo ftok");
        exit(EXIT_FAILURE);
    }

    mem = shmget(key, sizeof(Info), SHM_W|SHM_R|IPC_CREAT|IPC_EXCL);
    if(mem == -1){
        perror("Error generando memoria compartida");
        exit(EXIT_FAILURE);
    }

    buff = shmat(mem, NULL, 0);
    if(buff == (void *) -1){
        perror("Error añadiendo la memoria");
        exit(EXIT_FAILURE);
    }
    buff->id = 0;

    if(signal(SIGUSR1, handle_SIGUSR1) == SIG_ERR){
        perror("Error asignando el manejador");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < n; i++){
        pid = fork();
        if(!pid){
            rutina_hijo();
            exit(EXIT_FAILURE);
        }
    }
    while(1){
        if(buff->id == n){
            break;
        }
        pause();
    }
    while(wait(NULL) != -1);
    shmdt(buff);
    shmctl(mem, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}


void handle_SIGUSR1(int sig){
    printf("Nombre: %s - Id: %d\n", buff->nombre, buff->id);
}

void rutina_hijo(){
    char nombre[80];
    sleep((int) randNum(1, 6));
    printf("Soy %d y voy a leer tu nombre:\n", getpid());
    fgets(buff->nombre, 80, stdin);
    buff->nombre[strcspn(buff->nombre, "\n")] = 0;
    buff->id++;
    shmdt(buff);
    kill(getppid(), SIGUSR1);
    exit(EXIT_SUCCESS);
}

void usage(){
    printf("Usage is: ./build/ejercicio2 <integer>\n");
}
