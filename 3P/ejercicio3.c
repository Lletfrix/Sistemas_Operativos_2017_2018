/**
 * @brief Ejercicio 3
 *
 * Este fichero contiene el código fuente del ejercicio 5 de la entrega.
 * @file ejercicio3.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 14-04-2018
 */
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

#define LET 26 /*!< Numero de letras en ascii*/
#define NUM 10 /*!< Numero de números en ascii*/
#define KEY1 1300 /*!< Numero para generar una key con ftok*/
#define KEY2 1400 /*!< Numero para generar una key con ftok*/
#define KEY3 1500 /*!< Numero para generar una key con ftok*/
#define KEY4 1600 /*!< Numero para generar una key con ftok*/
#define PATH "/bin/bash" /*!< Path para generar una key con ftok*/

/**
* @brief Array de productos
*
* Array de chars en los que se guardarán y leeran los productos
*/
typedef struct productos{
    char p[LET+NUM]; /*!< Array de productos*/
} Productos;

Productos *buff = NULL; /*!< Variable global donde se guardan los productos*/
int semshm /*!< Controla el acceso a la memoria compartida*/;
int vacio /*!< Controla que el productor no produzca si el sitio esta lleno*/;
int lleno /*!< Controla que el consumidor no consuma si el sitio esta vacio*/;

/**
 * @brief Rutina que sigue el proceso indicado como productor
 *
 * Genera todas las letras y números del ascii en la memoria compartida
 */
void productor();

/**
 * @brief Rutina que sigue el proceso indicado como consumidor.
 *
 * Lee e imprime los caracteres escritos por el productor en la memoria compartida
 */
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

    val[0]=LET+NUM;
    if(ERROR == inicializar_semaforo(vacio, val)){
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
    borrar_semaforo(vacio);
    borrar_semaforo(lleno);
    exit(EXIT_SUCCESS);
}

void productor(){
    int i;
    for(i = 0; i < LET;){
        if (ERROR == down_semaforo(vacio, 0, 0)){
            perror("Error al bajar el semaforo vacio");
        }
        if (ERROR == down_semaforo(semshm, 0, 0)){
            perror("Error al bajar el semaforo semshm");
        }
        buff->p[i] = (char) i + 'A';
        ++i;
        if (ERROR == up_semaforo(semshm, 0, 0)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(lleno, 0, 0)){
            perror("Error al subir el semaforo lleno");
        }
    }
    for(i = LET; i < LET + NUM;){
        if (ERROR == down_semaforo(vacio, 0, 0)){
            perror("Error al bajar el semaforo vacio");
        }
        if (ERROR == down_semaforo(semshm, 0, 0)){
            perror("Error al bajar el semaforo semshm");
        }
        buff->p[i] = (char) i - LET + '0';
        ++i;
        if (ERROR == up_semaforo(semshm, 0, 0)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(lleno, 0, 0)){
            perror("Error al subir el semaforo lleno");
        }
    }
    shmdt(buff);
    exit(EXIT_SUCCESS);
}

void consumidor(){
    //int ind = 0;
    int i;
    for(i = 0; i < LET+NUM;){
        if (ERROR == down_semaforo(lleno, 0, 0)){
            perror("Error al bajar el semaforo lleno");
        }
        if (ERROR == down_semaforo(semshm, 0, 0)){
            perror("Error al bajar el semaforo semshm");
        }
        printf("Producto obtenido : %c\n", buff->p[i]);
        ++i;
        if (ERROR == up_semaforo(semshm, 0, 0)){
            perror("Error al subir el semaforo semshm");
        }
        if (ERROR == up_semaforo(vacio, 0, 0)){
            perror("Error al subir el semaforo vacio");
        }
    }
    shmdt(buff);
    exit(EXIT_SUCCESS);
}
