#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROC 3
#define MAX_MEM 2048
#define KEY 1300
#define PATH "/bin/bash"

void a(char *f_in, pid_t pid);
void b(pid_t pid_aux);
void c(char *f_out);
void usage();
void handler_sigusr1(int sig);
void handler_sigusr2(int sig);
void shift_letras(char *mensaje);

struct message{
    long mtype;
    char texto[MAX_MEM/sizeof(char)];
};

static volatile short a_is_running = 1;
static volatile short b_is_running = 1;
static int qid;

int main(int argc, char *argv[]) {
    int i;
    pid_t pid, pid_aux;
    if(argc != 3){
        usage();
        exit(EXIT_SUCCESS);
    }
    if(qid = msgget(ftok(PATH, KEY), IPC_CREAT|0600) == -1){
        perror("Error al crear la cola de mensajes");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < NUM_PROC; ++i){
        if(!(pid = fork())){
            switch (i) {
                case 0:
                    c(argv[2]);
                    exit(EXIT_FAILURE);
                case 1:
                    b(pid_aux);
                    exit(EXIT_FAILURE);
                case 2:
                    a(argv[1], pid_aux);
                    exit(EXIT_FAILURE);
                default:
                    exit(EXIT_FAILURE);
            }
        }
        pid_aux = pid;
    }
    while(wait(NULL) != -1);
    msgctl(qid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
void a(char *f_in, pid_t pid){
    FILE* fp;
    struct message mensaje;
    fp = fopen(f_in, "r");
    if(!fp){
        perror("Error abriendo archivo de lectura");
        exit(EXIT_FAILURE);
    }
    mensaje.mtype = 1;
    while(fread(mensaje.texto, MAX_MEM, 1, fp)){
        if(-1 == msgsnd(qid, &mensaje, MAX_MEM/sizeof(char), 0)){
            perror("Error mandando mensaje");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    kill(pid, SIGUSR1);
    fclose(fp);
    exit(EXIT_SUCCESS);
}

void b(pid_t pid_aux){
    short ret;
    /*struct message mensaje_recibido;
    struct message mensaje_enviado;*/
    struct message mensaje;
    signal(SIGUSR1, handler_sigusr1);
    while(a_is_running || (ret = msgrcv(qid, &mensaje, MAX_MEM/sizeof(char), 1, IPC_NOWAIT)) != -1){
        if(ret == -1){
            continue;
        }
        mensaje.mtype = 2;
        shift_letras(mensaje.texto);
        if(-1 == msgsnd(qid, &mensaje, MAX_MEM/sizeof(char), 0)){
            perror("Error mandando mensaje");
            exit(EXIT_SUCCESS);
        }
    }
    kill(pid_aux, SIGUSR2);
    exit(EXIT_SUCCESS);
}

void c(char *f_out){
    struct message mensaje;
    signal(SIGUSR2, handler_sigusr2);
    short ret;
    FILE *fp = fopen(f_out, "w");
    while(b_is_running || (ret = msgrcv(qid, &mensaje, MAX_MEM/sizeof(char), 2, IPC_NOWAIT)) != -1){
        if(ret == -1){
            continue;
        }
        fprintf(fp, "%s", mensaje.texto);
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}

void shift_letras(char *mensaje){
    while(*mensaje){
        //*mensaje = (*mensaje - 'a' + 1) % ('z' - 'a' + 1) + 'a';
        if(*mensaje >= 'A' && *mensaje < 'Z'){
            *mensaje = (*mensaje - 'A' + 1) % ('Z' - 'A' + 1) + 'A';
        } else if(*mensaje >= 'a' && *mensaje >= 'z'){
            *mensaje = (*mensaje - 'a' + 1) % ('z' - 'a' + 1) + 'a';
        }
        mensaje++;
    }
}

void usage(){
    printf("Usage is: ./build/ejercicio5 <filename1> <filename2>\n");
}

void handler_sigusr1(int sig){
    a_is_running = 0;
}

void handler_sigusr2(int sig){
    b_is_running = 0;
}
