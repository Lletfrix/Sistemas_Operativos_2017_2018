/**
 * @brief Ejercicio 5
 *
 * Este fichero contiene el código fuente del ejercicio 5 de la entrega.
 * @file ejercicio5.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 14-04-2018
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define NUM_PROC 3 /*!< Número de procesos*/
#define MAX_MEM 2048 /*!< Tamaño máximo de la información del mensaje*/
#define KEY 1300 /*!< Número para generar una key con ftok*/
#define PATH "/bin/bash" /*!< Path para generar una key con ftok*/

/**
 * @brief Realiza la función del proceso A descrito en la práctica
 *
 * Trocea el fichero en bloques de 2 kB (Salvo el último, que puede no serlo) y los manda como mensajes.
 */
void a(char *f_in);

/**
 * @brief Realiza la función del proceso B descrito en la práctica.
 *
 * Recibe los mensajes de A, los codifica con shift_letras y los manda como mensajes otra vez.
 */
void b(pid_t pid_aux);

/**
 * @brief Realiza la función del proceso C descrito en la práctica.
 *
 * Recibe los mensajes de B y los guarda en un fichero de texto.
 */
void c(char *f_out, pid_t pid_aux);

/**
 * @brief Imprime el uso correcto del ejercicio 5.
 *
 * Imprime el orden de los parametros de entrada del ejercicio 5.
 */
void usage();

/**
 * @brief Codifica los mensajes según lo descrito en la práctica
 *
 * Hace un Cifrado César con clave 1 al mensaje recibido.
 */
void shift_letras(char *mensaje);

/**
* @brief Estructura personalizada de mensaje
*
* Esta estructura guarda el tipo de mensaje y un array de chars con la información.
*/
struct message{
    long mtype; /*!< Tipo de mensaje*/
    char texto[MAX_MEM/sizeof(char)]; /*!< Información del mensaje*/
};

static volatile short a_is_running = 1;
static volatile short b_is_running = 1;
static int qid;

int main(int argc, char *argv[]) {
    int i;
    pid_t pid, pid_aux, pids[NUM_PROC];
    if(argc != 3){
        usage();
        exit(EXIT_SUCCESS);
    }
    if((qid = msgget(ftok(PATH, KEY), IPC_CREAT|0600)) == -1){
        perror("Error al crear la cola de mensajes");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", qid);
    for(i = 0; i < NUM_PROC; ++i){
        if(!(pid = fork())){
            switch (i) {
                case 0:
                    printf("Soy A: %d\n", getpid());
                    a(argv[1]);
                    exit(EXIT_FAILURE);
                case 1:
                    printf("Soy B: %d\n", getpid());
                    b(pid_aux);
                    exit(EXIT_FAILURE);
                case 2:
                    printf("Soy C: %d\n", getpid());
                    c(argv[2], pid_aux);
                    exit(EXIT_FAILURE);
                default:
                    exit(EXIT_FAILURE);
            }
        }
        pid_aux = pid;
        pids[i] = pid;
    }
    while(wait(NULL) != -1);
    msgctl(qid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
void a(char *f_in){
    FILE* fp;
    struct message mensaje = {0};
    fp = fopen(f_in, "r");
    if(!fp){
        perror("Error abriendo archivo de lectura");
        exit(EXIT_FAILURE);
    }
    mensaje.mtype = 1;
    while(!feof(fp)){
        fread(mensaje.texto, MAX_MEM, 1, fp);
        if(-1 == msgsnd(qid,(struct msgbuf *) &mensaje, MAX_MEM/sizeof(char), 0)){
            perror("Error mandando mensaje");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        memset(mensaje.texto,0,sizeof(mensaje.texto));
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}

void b(pid_t pid_aux){
    short ret;
    struct message mensaje = {0};
    while((ret = msgrcv(qid,(struct msgbuf *) &mensaje, MAX_MEM/sizeof(char), 1, IPC_NOWAIT))){
        if(errno == ENOMSG){
            if(kill(pid_aux, 0)){
                break;
            }
            errno = !ENOMSG;
            continue;
        }
        printf("El mensaje recibido por B es de tipo: %ld\n", mensaje.mtype);
        mensaje.mtype = 2;
        shift_letras(mensaje.texto);
        if(-1 == msgsnd(qid,(struct msgbuf *)  &mensaje, MAX_MEM/sizeof(char), 0)){
            perror("Error mandando mensaje");
            exit(EXIT_FAILURE);
        }
        memset(mensaje.texto,0,sizeof(mensaje.texto));
    }
    exit(EXIT_SUCCESS);
}

void c(char *f_out, pid_t pid_aux){
    struct message mensaje = {0};
    short ret;
    FILE *fp = fopen(f_out, "w");
    while((ret = msgrcv(qid,(struct msgbuf *) &mensaje, MAX_MEM/sizeof(char), 2, IPC_NOWAIT))){
        if(errno == ENOMSG){
            if(kill(pid_aux, 0)){
                break;
            }
            errno = !ENOMSG;
            continue;
        }
        printf("El mensaje recibido por C es de tipo: %ld\n", mensaje.mtype);
        fprintf(fp, "%s", mensaje.texto);
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}

void shift_letras(char *mensaje){
    while(*mensaje){
        if(*mensaje >= 'A' && *mensaje < 'Z'){
            *mensaje = (*mensaje - 'A' + 1) % ('Z' - 'A' + 1) + 'A';
        } else if(*mensaje >= 'a' && *mensaje <= 'z'){
            *mensaje = (*mensaje - 'a' + 1) % ('z' - 'a' + 1) + 'a';
        }
        mensaje++;
    }
}

void usage(){
    printf("Usage is: ./build/ejercicio5 <filename1> <filename2>\n");
}
