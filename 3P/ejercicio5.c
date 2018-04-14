#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define NUM_PROC 3
#define MAX_MEM 2048
#define KEY 1300
#define PATH "/bin/bash"

void a(char *f_in, pid_t pid);
void b();
void c(char *f_out);
void usage();

struct message{
    long mtype;
    char texto[MAX_MEM/sizeof(char)];
};

static volatile short a_is_running;

int main(int argc, char const *argv[]) {
    int i, qid;
    pit_t pid, pid_aux;
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
                    b();
                    exit(EXIT_FAILURE);
                case 1:
                    a(argv[1], pid_aux);
                    exit(EXIT_FAILURE);
                case 2:
                    c(argv[2]);
                    exit(EXIT_FAILURE);
                default:
                    exit(EXIT_FAILURE);
            }
        }
        pid_aux = pid;
    }
    while(wait(NULL) != -1);
    exit(EXIT_SUCCESS);
}
void a(char *f_in, pid_t pid){
    FILE* fp;
    struct message mensaje;
    fp = fopen(f_in);
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

void b(){
    /*struct message mensaje_recibido;
    struct message mensaje_enviado;*/
    struct message mensaje;
    while(a_is_running){
        if(-1 == msgrcv(qid, &mensaje, MAX_MEM/sizeof(char), 1, 0)){
            perror("Error recibiendo mensaje");
            exit(EXIT_FAILURE);
        }
        mensaje.mtype = 2;
        shift_letras(mensaje.texto);
        if(-1 == msgsnd(qid, &mensaje, MAX_MEM/sizeof(char), 0)){
            perror("Error mandando mensaje");
        }
    }
}

void shift_letras(char *mensaje){
    while(*mensaje){
        *mensaje = (*mensaje - 'a' + 1) % ('z' - 'a' + 1) + 'a';
        mensaje++;
    }
}

void usage(){
    printf("Usage is: ./build/ejercicio5 <filename1> <filename2>\n");
}

void handler_sigusr1(int sig){
    a_is_running = 0;
}
