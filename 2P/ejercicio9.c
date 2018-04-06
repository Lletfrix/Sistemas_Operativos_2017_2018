/**
 * @brief Ejercicio 9
 *
 * Este fichero contiene el código fuente del ejercicio 9 de la entrega.
 * @file ejercicio9.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 06-04-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "semaforos.h"
#include "mysignal.h"

#define NUM_CAJ 3 /*!< Numero de procesos hijo/cajas.*/
#define NUM_OPER 50 /*!< Numero de operaciones de cada caja*/
#define KEY 2018 /*!< Numero para generar una key con ftok*/
#define PATH "/bin/bash" /*!< Path para generar una key con ftok*/
#define SIGMONEY SIGRTMIN /*!< Señal para indicar de que se tienen mas de 1000€*/
#define SIGDONE SIGRTMIN+1 /*!< Señal para indicar la finalizacion de la caja*/
#define TEXTDIR "text/" /*!< Directorio de los ficheros de transacciones*/
#define DATDIR "dat/" /*!< Directorio de los ficheros de datos*/
#define LEE 1 /*!< Flag para dinero_recogido*/
#define ESCRIBE 0 /*!< Flag para dinero_recogido*/
#define MAXBUFF 256 /*!< Tamaño maximo de la cadena de caracteres*/

static volatile int mutex_hijo, terminados;
static volatile float cuenta = 0;

 /**
  * @brief Dado un id, procesa todas las transacciones asociadas a el.
  *
  * Abre su fichero de transacciones correspondiente y las va procesando
  * en otro fichero, avisando al padre cuando supere la cantidad estipulada.
  *
  * Corresponde a la rutina del proceso hijo.
  *
  * @param id Identificador del fichero de transacciones.
  */
 void cajero(int id);

/**
 * @brief Lee  de o escribe en el fichero de nombre filename un booleano.
 *
 * Abre el archivo de nombre filename, dependiendo de flag leera un booleano
 * o escribira dinero_sacado. Cierra el fichero.
 *
 * @param filename String con el nombre del archivo.
 * @param dinero_sacado Valor bool a escribir. Este parametro se ignora si flag es LEE.
 * @param flag Bandera de valor LEE o ESCRIBE que determinara el comportamiento de la funcion.
 * @return Devuelve el valor del bool de filename si se especifica LEE. Si se especifica ESCRIBE
 * se devuelve false en caso de error, true en otro caso.
 */
bool dinero_recogido(char *filename, bool dinero_sacado, int flag);

/**
 * @brief Manejador de la señal SIGDONE (SIGRTMIN+1)
 *
 * Retira todo el dinero que quede en la caja del proceso hijo recibido con
 * la señal e incrementa el contador de procesos terminados.
 *
 * @param sig Señal recibida.
 * @param info Puntero a la estructura siginfo_t con informacion. Especificada en el manual de sigaction(3).
 * @param vp Puntero a la estructura ucontext_t. Mas informacion en el manual de getcontext(1).
 */
void handle_SIGDONE(int sig, siginfo_t *info, void *vp);

/**
 * @brief Manejador de la señal SIGMONEY (SIGRTMIN).
 *
 * Reserva el archivo de donde retirara el dinero pertinente, bloqueandose cuando
 * ya este reservado.
 *
 * @param sig Señal recibida.
 * @param info Puntero a la estructura siginfo_t con informacion. Especificada en el manual de sigaction(3).
 * @param vp Puntero a la estructura ucontext_t. Mas informacion en el manual de getcontext(1).
 */
void handle_SIGMONEY(int sig, siginfo_t *info, void *vp);

/**
 * @brief Suma delta al float existente en el archivo de nombre filename
 *
 * Se abre el archivo de nombre filename, y se escribe en el el resultado de sumar
 * delta a el float leido del archivo.
 *
 * @param filename String con el nombre del archivo.
 * @param delta Valor en floating point a sumar.
 * @return Devuelve ERROR si falla o el float actualizado en exito.
 */
 float increase_subtotal(char *filename, float delta);

/**
 * @brief Genera un número real aleatorio en el rango [inf, sup)
 *
 * Genera un número real aleatorio en el rango [inf, sup)
 *
 * @param inf Límite inferior del intervalo
 * @param sup Límite superior del intervalo
 * @return Devuelve el número aleatorio generado
 */
float randNum(float inf, float sup);


int main(int argc, char const *argv[]) {
    int i, j, fake_semid;
    pid_t pid;
    char filename[MAXBUFF];
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
        borrar_semaforo(mutex_hijo);
        exit(EXIT_FAILURE);
    }

    /* Generación de ficheros de transacciones */
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
        borrar_semaforo(mutex_hijo);
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

bool dinero_recogido(char *filename, bool dinero_sacado, int flag){
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
        fclose(fp);
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

void handle_SIGMONEY(int sig, siginfo_t *info, void *vp){
    int caja;
    char filename[MAXBUFF];
    caja = info->si_int;
    sprintf(filename, DATDIR "caja%d.dat", caja+1);
    down_semaforo(mutex_hijo, caja, SEM_UNDO);
    if(ERROR == increase_subtotal(filename, -900)){
        fprintf(stderr, "Cannot open file\n");
    };
    cuenta += 900;
    dinero_recogido(filename, true, ESCRIBE);
    up_semaforo(mutex_hijo, caja, SEM_UNDO);
    return;
}

void handle_SIGDONE(int sig, siginfo_t *info, void *vp){
    int caja;
    float dinero;
    char filename[MAXBUFF];
    FILE *fp;
    caja = info->si_int;
    sprintf(filename, DATDIR "caja%d.dat", caja+1);
    fp = fopen(filename, "r+");
    fread(&dinero, sizeof(float), 1,fp);
    cuenta += dinero;
    dinero = 0;
    fseek(fp, 0, SEEK_SET);
    fwrite(&dinero, sizeof(float), 1, fp);
    fclose(fp);
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
    char filename[MAXBUFF], filename_caja[MAXBUFF], price[8];
    float p;
    union sigval val = {0};
    bool booleano;

    /* Abre el archivo de transacciones y crea el archivo del dinero
       de la caja */
    sprintf(filename, TEXTDIR "clientesCaja%d.txt", id+1);
    fp = fopen(filename, "r");
    sprintf(filename_caja, DATDIR "caja%d.dat", id+1);
    fp_caja = fopen(filename_caja, "w");
    dinero_recogido(filename_caja, true, ESCRIBE);
    fclose(fp_caja);

    /* Rellena el entero que enviaremos con sigqueue */
    val.sival_int = id;

    while(fgets(price, sizeof(price), fp)){
        p = atof(price);
        sleep((int) randNum(1, 6));
        down_semaforo(mutex_hijo, id, SEM_UNDO);
        if(ERROR == (p = increase_subtotal(filename_caja, p))){
            fprintf(stderr, "Cannot open file\n");
        };
        if(p > 1000 && (booleano = dinero_recogido(filename_caja, true, LEE)) == true){
            sigqueue(getppid(), SIGMONEY, val);
            dinero_recogido(filename_caja, false, ESCRIBE);
        }
        up_semaforo(mutex_hijo, id, SEM_UNDO);
    }
    sigqueue(getppid(), SIGDONE, val);
    fclose(fp);
    exit(EXIT_SUCCESS);
}
