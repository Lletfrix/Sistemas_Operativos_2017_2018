#include <signal.h>
#include "sim_carr_lib.h"


void _manejador_SIGTHROW(int sig);

void proc_tirada(int id, int **pipe){
    sigset_t set, oldset;
    char tirada_type;
    unsigned short tirada;
    struct msgtir mensaje;
    int msgqid;

    close(pipe[id][WRITE]);
    signal(SIGTHROW, _manejador_SIGTHROW);
    signal(SIGINT, _manejador_SIGINT);
    signal(SIGFREE, _manejador_SIGFREE);
    sigemptyset(set);
    sigaddset_var(set, SIGTHROW, SIGINT, SIGFREE, -1);
    sigprocmask(SIG_BLOCK, set, oldset);
    while(1){
        tirada = 0;
        sigsuspend(oldset);
        read(pipe[id][READ], &tirada_type, sizeof(char));
        switch (tirada_type) {
            case REMONTAR:
                tirada += (unsigned short) randNum(1, 7);
                tirada += (unsigned short) randNum(1, 7);
            break;
            case GANADORA:
                tirada += (unsigned short) randNum(1, 8);
            break;
            case NORMAL:
                tirada += (unsigned short) randNum(1, 7);
            break;
            default:
        }
        msgqid = msgget(ftok(PATH, KEY_CAB_SHM), 0);
        mensaje.mtype = id;
        mensaje.tirada = tirada;
        msgsnd(msgqid, mensaje, sizeof(unsigned short), id, 0);
    }


}

void _manejador_SIGTHROW(int sig){
    return;
}
