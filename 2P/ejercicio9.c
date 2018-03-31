#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "semaforos.h"
#include "mysignal.h"

#define NUM_CAJ 25
#define NUM_OPER 50
#define KEY 2018
#define SIGMONEY SIGUSR1
#define TEXTDIR "text/"
#define DATDIR "dat/"

static volatile int mutex_hijo, terminados;

int increase_subtotal(char *filename, float delta){
    FILE *fp;
    float dinero;
    fp = fopen(filename, "r+");
    if(!fp){
        return ERROR;
    }
    fread(&dinero, sizeof(float), 1, fp);
    dinero -= 900;
    fseek(fp, 0, SEEK_SET);
    fwrite(&dinero, sizeof(float), 1, fp);
    fclose(fp);
    return OK;
}

void handle_SIGMONEY(int sig, siginfo_t *info, void *vp){
    int caja;
    char *filename;
    caja = info->si_int;
    filename = (char *)info->si_ptr;
    while(down_semaforo(mutex_hijo, caja, IPC_NOWAIT));
        if(ERROR == increase_subtotal(filename, -900)){
            fprintf(stderr, "Cannot open file\n");
        };
    while(up_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    return;
}

void handle_SIGCLD(int sig, siginfo_t *info, void *vp){
    int caja, dinero;
    char *filename;
    FILE *fp;
    caja = info->si_int;
    filename = (char *)info->si_ptr;
    while(down_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    fp = fopen(filename, "r+");
    dinero = 0;
    fseek(fp, 0, SEEK_SET);
    fwrite(&dinero, sizeof(int), 1, fp);
    fclose(fp);
    while(up_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    terminados++;
}

float randNum(float inf, float sup){
    float temp, r;
    if (!(temp = sup - inf)){
        return temp;
    }else if(temp < 0){
        temp = inf;
        inf = sup;
        sup = temp;
    }
    r = (float) inf + rand() / (RAND_MAX / (sup - inf) + 1.0);
    return r;
}

void cajero(int id){
    FILE *fp, *fp_caja;
    char filename[256], filename_caja[256], price[8];
    float p;

    sprintf(filename, TEXTDIR "clientesCaja%d.txt", id+1);
    fp = fopen(filename, "r");
    sprintf(filename_caja, DATDIR "caja%d.dat", id+1);
    fp_caja = fopen(filename_caja, "w");
    fclose(fp_caja);
    while(fgets(price, sizeof(price), fp)){
        p = atof(price);
        sleep((int) randNum(1, 6));
        while(down_semaforo(mutex_hijo, id, IPC_NOWAIT));

        while(up_semaforo(mutex_hijo, id, IPC_NOWAIT));
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    int i, j, fake_semid;
    pid_t pid;
    char filename[256];
    sigset_t mask;
    FILE *f = NULL;
    struct sigaction get_900, end_caja;

    get_900.sa_sigaction = handle_SIGMONEY;
    get_900.sa_flags = SA_SIGINFO;
    sigemptyset(&get_900.sa_mask);
    sigaddset(&get_900.sa_mask, SIGCLD);
    if(sigaction(SIGMONEY, &get_900, NULL) == -1){
        printf("Error al cambiar el manejador de SIGUSR1: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    end_caja.sa_sigaction = handle_SIGCLD;
    end_caja.sa_flags = SA_SIGINFO;
    sigemptyset(&end_caja.sa_mask);
    sigaddset(&end_caja.sa_mask, SIGMONEY);
    if(sigaction(SIGCLD, &end_caja, NULL) == -1){
        printf("Error al cambiar el manejador de SIGCLD: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sigemptyset(&mask);
    sigaddset_var(&mask, SIGMONEY, SIGCLD, -1);

    if(ERROR == crear_semaforo(KEY, NUM_CAJ, &fake_semid)){
        exit(EXIT_FAILURE);
    }
    mutex_hijo = fake_semid;


    /* Generate transaction files */
    for (i = 0; i < NUM_CAJ; ++i) {
        sprintf(filename, TEXTDIR "clientesCaja%d.txt", i+1);
        f = fopen(filename, "w");
        if (!f){
            exit(EXIT_FAILURE);
        }
        for (j = 0; j < NUM_OPER; ++j){
            fprintf(f, "%.2f\n", randNum(0, 300.005));
        }
        fclose(f);
    }

    for (i = 0; i < NUM_CAJ; ++i){
        pid = fork();
        if(!pid){
            break;
        }
    }

    if(!pid){
        cajero(i);
    }

    terminados = 0;
    while (terminados < NUM_CAJ) {
        sigsuspend(&mask);
    }

    while(wait(NULL) != -1);
    exit(EXIT_SUCCESS);
}
