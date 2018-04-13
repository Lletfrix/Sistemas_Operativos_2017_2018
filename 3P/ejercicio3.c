#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/sem.h>
#include <ctype.h>

#include "mylib.h"
#include "semaforos.h"

#define LET 26
#define NUM 10
#define KEY1 1300
#define KEY2 1400
#define KEY3 1500
#define KEY4 1600
#define PATH "/bin/bash"

typedef struct productos{
    char p[LET+NUM];
} Productos;

Productos *buff = NULL;
int semshm, vacio, lleno;

void productor();

void consumidor();

int main(int argc, char const *argv[]) {
    key_t key1, key2, key3, key4;
    int mem, i;
    pid_t pid;
    unsigned short val[1];

    if((key1 = ftok(PATH, KEY1)) == -1){
        perror("Fallo ftok");
        exit(EXIT_FAILURE);
    }

    if((key2 = ftok(PATH, KEY2)) == -1){
        perror("Fallo ftok");
        exit(EXIT_FAILURE);
    }

    if((key3 = ftok(PATH, KEY3)) == -1){
        perror("Fallo ftok");
        exit(EXIT_FAILURE);
    }

    if((key4 = ftok(PATH, KEY4)) == -1){
        perror("Fallo ftok");
        exit(EXIT_FAILURE);
    }

    if(ERROR == crear_semaforo(key1, 1, &semshm)){
        perror("Error creando semaforo");
        exit(EXIT_FAILURE);
    }

    if(ERROR == crear_semaforo(key2, 1, &vacio)){
        perror("Error creando semaforo");
        borrar_semaforo(semshm);
    }

    if(ERROR == crear_semaforo(key3, 1, &lleno)){
        perror("Error creando semaforo");
        borrar_semaforo(semshm);
        borrar_semaforo(vacio);
    }

    mem = shmget(key4, sizeof(Productos), SHM_W|SHM_R|IPC_CREAT);
    if(mem == -1){
        perror("Error generando memoria compartida");
        borrar_semaforo(semshm);
        borrar_semaforo(vacio);
        borrar_semaforo(lleno);
        exit(EXIT_FAILURE);
    }

    buff = shmat(mem, NULL, 0);
    if(buff == (void *) -1){
        perror("Error añadiendo la memoria");
        borrar_semaforo(semshm);
        borrar_semaforo(vacio);
        borrar_semaforo(lleno);
        shmctl(mem, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    val[0] = 1;
    if(ERROR == inicializar_semaforo(semshm, val)){
        perror("Error al inicializar el semaforo");
    }

    if(ERROR == inicializar_semaforo(vacio, val)){
        perror("Error al inicializar el semaforo");
    }

    if(ERROR == inicializar_semaforo(lleno, val)){
        perror("Error al inicializar el semaforo");
    }

    pid = fork();
    if(!pid){
        productor();
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if(!pid){
        consumidor();
        exit(EXIT_FAILURE);
    }

    while(wait(NULL) != -1);
    shmdt(buff);
    shmctl(mem, IPC_RMID, NULL);
    borrar_semaforo(semshm);
    exit(EXIT_SUCCESS);

}

void productor(){
    int ind = 0;
    int i;
    for(i = (int)'A'; i < LET + (int)'A'; i++){
        if (ERROR == down_semaforo(vacio, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo vacio");
        }
        if (ERROR == down_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo semshm");
        }
        buff->p[ind] = (char)i;
        ++ind;
        if (ERROR == up_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(lleno, 0, SEM_UNDO)){
            perror("Error al subir el semaforo lleno");
        }
    }
    for(i = (int)'0'; i < NUM + (int)'0'; i++){
        if (ERROR == down_semaforo(vacio, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo vacio");
        }
        if (ERROR == down_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo semshm");
        }
        buff->p[ind] = (char)i;
        ++ind;
        if (ERROR == up_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(lleno, 0, SEM_UNDO)){
            perror("Error al subir el semaforo lleno");
        }
    }
    shmdt(buff);
    exit(EXIT_SUCCESS);
}

void consumidor(){
    int ind = 0;
    int i;
    for(i = 0; i < LET+NUM; i++){
        if (ERROR == down_semaforo(lleno, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo lleno");
        }
        if (ERROR == down_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al bajar el semaforo semshm");
        }
        printf("Producto obtenido : %c\n", buff->p[ind]);
        ++ind;
        if (ERROR == up_semaforo(semshm, 0, SEM_UNDO)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(vacio, 0, SEM_UNDO)){
            perror("Error al subir el semaforo vacio");
        }
    }
    shmdt(buff);
    exit(EXIT_SUCCESS);
}
