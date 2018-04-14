/**
 * @brief Ejercicio 4
 *
 * Este fichero contiene el código fuente del ejercicio 4 de la entrega.
 * @file ejercicio4.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 14-04-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "mylib.h"
#include "semaforos.h"

#define NUM_HILOS 2 /*!< Numero de hilos*/
#define LECTOR 0 /*!< Numero que representa al lector*/
#define ESCRITOR 1 /*!< Numero que representa al escritor*/
#define MIN_NUM 1000 /*!< Mínimo número de números a generar*/
#define MAX_NUM 2000 /*!< Máximo número de números a generar*/
#define MIN_RAND 100 /*!< Mínimo número aleatorio*/
#define MAX_RAND 1000 /*!< Máximo número aleatorio*/
#define MAX_BUF 256 /*!< Tamaño máximo del buffer*/

/**
* @brief Estructura para pasar información a los threads
*
* Contiene la información que necesitamos transmitir del padre al thread.
*/
struct escritura{
    int nnum; /*!< Número de números a generar*/
    char pathname[MAX_BUF]; /*!< Pathname del fichero donde escribir los números*/
};

/**
 * @brief Función que nos permite escribir en un fichero los números aleatorios requeridos
 *
 * Escribe en el fichero indicado por pathname nnum números aleatorios
 */
void* escribir(void* escr);

/**
 * @brief Función que mapea un fichero y lee su información
 *
 * Mapea el fichero indicado por fdesc a memoria y lee su información desde ahí
 */
void* leer(void* fdesc);

/**
 * @brief Imprime el uso correcto del ejercicio 4.
 *
 * Imprime el orden de los parametros de entrada del ejercicio 4.
 */
void usage();

int main(int argc, char const *argv[]) {
    pthread_t hilos[NUM_HILOS];
    unsigned int fd;
    struct escritura esc;
    if(argc != 2){
        usage();
        exit(EXIT_FAILURE);
    }
    esc.nnum = randNum(MIN_NUM, MAX_NUM + 1);;
    strcpy(esc.pathname, argv[1]);
    pthread_create(&hilos[ESCRITOR], NULL, escribir, &esc);
    pthread_join(hilos[ESCRITOR], NULL);
    fd = open(argv[1], O_RDWR);
    pthread_create(&hilos[LECTOR], NULL, leer, &fd);
    pthread_join(hilos[LECTOR], NULL);
    close(fd);
    exit(EXIT_SUCCESS);
}

void* escribir(void* escr){
    int i;
    struct escritura *e = (struct escritura *) escr;
    FILE* fp;
    fp = fopen(e->pathname, "w");
    if(!fp){
        perror("Error abriendo fichero");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < e->nnum; ++i){
        fprintf(fp, "%d", (int) randNum(MIN_RAND, MAX_RAND + 1));
        if(i != e->nnum - 1){
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "%c", 0);
    fclose(fp);
    return NULL;
}

void* leer(void *fdesc){
    size_t size;
    char *addr, *aux;
    unsigned int fd = *(unsigned int*) fdesc;
    struct stat status;
    if(-1 == fstat(fd, &status)){
        perror("Error al obtener los datos del fd");
        exit(EXIT_FAILURE);
    }
    size = status.st_size;
    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(addr == MAP_FAILED){
        perror("Error al mapear la memoria");
        exit(EXIT_FAILURE);
    }
    aux = addr;
    while (*aux) {
        if(*aux == ','){
            *aux = ' ';
        }
        ++aux;
    }
    printf("%s\n", addr);
    munmap(addr, 0);
    return NULL;
}

void usage(){
    printf("Usage is: ./build/ejercicio4 <pathname>\n");
}
