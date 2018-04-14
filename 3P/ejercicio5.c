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

#define NUM_PROC 3
#define MAX_MEM 2048
#define KEY 1300
#define PATH "/bin/bash"

void a(char *f_in);
void b(pid_t pid_aux);
void c(char *f_out, pid_t pid_aux);
void usage();
void handler_sigusr1(int sig);
/*void handler_sigusr2(int sig);*/
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
    pid_t pid, pid_aux, pids[NUM_PROC];
    void handler_sigusr1();
    if(argc != 3){
        usage();
        exit(EXIT_SUCCESS);
    }
    if((qid = msgget(ftok(PATH, KEY), IPC_CREAT|0600)) == -1){
        perror("Error al crear la cola de mensajes");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", qid);
    signal(SIGUSR1, handler_sigusr1);
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
    /*for(i = 0; i < NUM_PROC; ++i){
        kill(pids[i], SIGUSR1);
    }*/
    while(wait(NULL) != -1);
    msgctl(qid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}
void a(char *f_in){
    FILE* fp;
    struct message mensaje = {0};
    //pause();
    fp = fopen(f_in, "r");
    //sleep(60);
    if(!fp){
        perror("Error abriendo archivo de lectura");
        exit(EXIT_FAILURE);
    }
    mensaje.mtype = 1;
    while(!feof(fp)){
        fread(mensaje.texto, MAX_MEM, 1, fp);
        if(-1 == msgsnd(qid,(struct msgbuf *) &mensaje, MAX_MEM/sizeof(char), 0)){
            fprintf(stderr, "Error mandando mensaje en el proceso: %d, msqid: %d, mtype: %ld, mtext\n %s\n Error: %s\n", getpid(), qid, mensaje.mtype, mensaje.texto, strerror(errno));
            //perror("Error mandando mensaje");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        printf("Se supone que he enviado el mensaje\n");
    }
    //kill(pid, SIGUSR1);
    fclose(fp);
    exit(EXIT_SUCCESS);
}

void b(pid_t pid_aux){
    short ret;
    /*struct message mensaje_recibido;
    struct message mensaje_enviado;*/
    struct message mensaje = {0};
    //signal(SIGUSR1, handler_sigusr1);
    //sleep(60);
    //pause();
    while((ret = msgrcv(qid,(struct msgbuf *) &mensaje, MAX_MEM/sizeof(char), 1, IPC_NOWAIT))){
        printf("%d\n", ret);
        printf("%s\n", mensaje.texto);
        if(errno == ENOMSG){
            if(kill(pid_aux, 0)){
                break;
            }
            errno = !ENOMSG;
            continue;
        }
        printf("El mensaje recibido por B es de tipo: %ld\n", mensaje.mtype);
        mensaje.mtype = 2;
        //printf("%s\n", mensaje.texto);
        shift_letras(mensaje.texto);
        //printf("%s\n", mensaje.texto);
        if(-1 == msgsnd(qid,(struct msgbuf *)  &mensaje, MAX_MEM/sizeof(char), 0)){
            fprintf(stderr, "Error mandando mensaje en el proceso: %d, msqid: %d, mtype: %ld, mtext\n %s\n Error: %s\n", getpid(), qid, mensaje.mtype, mensaje.texto, strerror(errno));
            //perror("Error mandando mensaje");
            exit(EXIT_FAILURE);
        }
    }
    //kill(pid_aux, SIGUSR2);
    exit(EXIT_SUCCESS);
}

void c(char *f_out, pid_t pid_aux){
    struct message mensaje = {0};
    short ret;
    //pause();
    //sleep(60);
    //signal(SIGUSR2, handler_sigusr2);
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
        //*mensaje = (*mensaje - 'a' + 1) % ('z' - 'a' + 1) + 'a';
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

void handler_sigusr1(int sig){
    return;
}
/*
void handler_sigusr2(int sig){
    b_is_running = 0;
}*/
