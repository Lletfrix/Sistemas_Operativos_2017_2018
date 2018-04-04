#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "semaforos.h"
#include "mysignal.h"

#define NUM_CAJ 2
#define NUM_OPER 50
#define KEY 2018
#define PATH "/bin/bash"
#define SIGMONEY SIGUSR1
#define SIGDONE SIGUSR2
#define TEXTDIR "text/"
#define DATDIR "dat/"
#define LEE 1
#define ESCRIBE 0

static volatile int mutex_hijo, terminados;
//static volatile bool dinero_sacado[NUM_CAJ];
static volatile float cuenta = 0;

/* Aumenta el total del archivo dato con el arguento delta */
float increase_subtotal(char *filename, float delta){
    FILE *fp;
    float dinero;
    fp = fopen(filename, "r+");
    if(!fp){
        return ERROR;
    }
    fread(&dinero, sizeof(float), 1, fp);
    dinero += delta;
    fseek(fp, 0, SEEK_SET);
    fwrite(&dinero, sizeof(float), 1, fp);
    fclose(fp);
    return dinero;
}

bool saca_dinero(char *filename, bool dinero_sacado, int flag){
    FILE *fp;
    bool retorno;
    fp = fopen(filename, "r+");
    if(!fp){
        printf("Cannot open file\n");
        return false;
    }
    fseek(fp, sizeof(float), SEEK_SET);
    if(flag == LEE){
        fread(&retorno, sizeof(bool), 1, fp);
        return retorno;
    }else if(flag == ESCRIBE){
        fwrite(&dinero_sacado, sizeof(bool), 1, fp);
    }else{
        printf("Argument error\n");
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

/* Majeador de SIGMONEY */
void handle_SIGMONEY(int sig, siginfo_t *info, void *vp){
    int caja;
    char filename[256];
    caja = info->si_int;
    printf("He entrado en SIGMONEY desde la caja %d\n", caja);
    sprintf(filename, DATDIR "caja%d.dat", caja+1);
    while(ERROR == down_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    if(ERROR == increase_subtotal(filename, -900)){
        fprintf(stderr, "Cannot open file\n");
    };
    cuenta += 900;
    printf("%f\n", cuenta);
    saca_dinero(filename, true, ESCRIBE);
    while(ERROR == up_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    return;
}

/* Majeador de SIGDONE */
void handle_SIGDONE(int sig, siginfo_t *info, void *vp){
    int caja;
    float dinero;
    char filename[256];
    FILE *fp;
    caja = info->si_int;
    printf("He entrado en SIGDONE desde la caja %d\n", caja);
    sprintf(filename, DATDIR "caja%d.dat", caja+1);
    //while(ERROR == down_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    fp = fopen(filename, "r+");
    fread(&dinero, sizeof(float), 1,fp);
    cuenta += dinero;
    dinero = 0;
    fseek(fp, 0, SEEK_SET);
    fwrite(&dinero, sizeof(float), 1, fp);
    fclose(fp);
    //while(ERROR == up_semaforo(mutex_hijo, caja, IPC_NOWAIT));
    terminados++;
}

/* Funcion generadora de numeros aleatorios en el rango [inf, sup) */
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

/* Rutina que ejecutará cada hijo. Recibe su id como argumento. */
void cajero(int id){
    FILE *fp, *fp_caja;
    char filename[256], filename_caja[256], price[8];
    float p;
    union sigval val;
    bool booleano;

    /* Abre el archivo de transacciones y crea el archivo del dinero
       de la caja */
    sprintf(filename, TEXTDIR "clientesCaja%d.txt", id+1);
    fp = fopen(filename, "r");
    sprintf(filename_caja, DATDIR "caja%d.dat", id+1);
    fp_caja = fopen(filename_caja, "w");
    saca_dinero(filename_caja, true, ESCRIBE);
    fclose(fp_caja);

    /* Rellena el entero que enviaremos con sigqueue */
    val.sival_int = id;

    //dinero_sacado[id] = true;
    while(fgets(price, sizeof(price), fp)){
        p = atof(price);
        sleep((int) randNum(1, 3));
        while(down_semaforo(mutex_hijo, id, IPC_NOWAIT));
        if(ERROR == (p = increase_subtotal(filename_caja, p))){
            fprintf(stderr, "Cannot open file\n");
        };
        printf("%f from id: %d\n", p, id);
        if(p > 1000 && (booleano = saca_dinero(filename_caja, true, LEE)) == true){
            sigqueue(getppid(), SIGMONEY, val);
            saca_dinero(filename_caja, false, ESCRIBE);
        }
        printf("DEBUG PURPOSE: Mi booleano es: %d from id %d\n", booleano, val.sival_int);
        while(up_semaforo(mutex_hijo, id, IPC_NOWAIT));
    }
    printf("DEBUG PURPOSE: He terminado con id: %d\n", val.sival_int);
    sigqueue(getppid(), SIGDONE, val);
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    int i, j, fake_semid;
    pid_t pid;
    char filename[256];
    sigset_t mask, o_mask;
    unsigned short mutex_initial[NUM_CAJ];
    FILE *f = NULL;
    struct sigaction get_900, end_caja;

    /* Set de manejadores y selañes */
    get_900.sa_sigaction = handle_SIGMONEY;
    get_900.sa_flags = SA_SIGINFO;
    sigemptyset(&get_900.sa_mask);
    sigaddset(&get_900.sa_mask, SIGDONE);
    if(sigaction(SIGMONEY, &get_900, NULL) == -1){
        printf("Error al cambiar el manejador de SIGUSR1: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    end_caja.sa_sigaction = handle_SIGDONE;
    end_caja.sa_flags = SA_SIGINFO;
    sigemptyset(&end_caja.sa_mask);
    sigaddset(&end_caja.sa_mask, SIGMONEY);
    if(sigaction(SIGDONE, &end_caja, NULL) == -1){
        printf("Error al cambiar el manejador de SIGDONE: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sigemptyset(&mask);
    sigaddset_var(&mask, SIGMONEY, SIGDONE, -1);

    /* Set de semaforos */

    if(ERROR == crear_semaforo(ftok(PATH, KEY), NUM_CAJ, &fake_semid)){
        printf("Error al crear el semaforo: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    mutex_hijo = fake_semid;

    for (i = 0; i<NUM_CAJ; ++i){
        mutex_initial[i]=1;
    }
    if(ERROR == inicializar_semaforo(mutex_hijo, mutex_initial)){
        printf("Error al inicializar el semaforo: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

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

    /* Creacion de procesos hijo */
    for (i = 0; i < NUM_CAJ; ++i){
        pid = fork();
        if(!pid){
            break;
        }
    }

    if(!pid){
        cajero(i);
        exit(EXIT_FAILURE);
    }

    terminados = 0;
    sigprocmask(SIG_BLOCK, &mask, &o_mask);
    while (terminados < NUM_CAJ) {
        sigsuspend(&o_mask);
    }
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    borrar_semaforo(mutex_hijo);
    while(wait(NULL) != -1);
    printf("El total recaudado es: %.2f €\n", cuenta);
    exit(EXIT_SUCCESS);
}
