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
#include <pthread.h>

#include "mylib.h"
#include "semaforos.h"

#define NUM_HILOS 2
#define LECTOR 0
#define ESCRITOR 1

struct _escritura{
    int nnum;
    char *pathname;
}

int main(int argc, char const *argv[]) {
    pthread_t hilos[NUM_HILOS];
    int nnum = randNum(1000,2001);
    int fd;
    pthread_create(&hilos[ESCRITOR], NULL, escribir, &nnum);
    pthread_join(hilos[ESCRITOR], NULL);
    fd = open(argv[1], O_RDWR);
    pthread_create(&hilos[LECTOR], NULL, leer, &fd);
    exit(EXIT_SUCCESS);
}
