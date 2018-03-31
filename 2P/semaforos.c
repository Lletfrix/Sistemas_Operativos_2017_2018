#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>

#include "semaforos.h"
int inicializar_semaforo(int semid, unsigned short *array){
    int retorno;
    //struct semid_ds semid_ds;
    union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
    }arg;

    //arg.buf = &semid_ds;
    arg.array = array;
    retorno = semctl(semid, 0, SETALL, arg);
    (retorno == -1) ? retorno = ERROR : OK;
    return retorno;
}

int borrar_semaforo(int semid){
    int retorno;
    retorno = semctl(semid, 0, IPC_RMID);
    (retorno == -1) ? retorno = ERROR : OK;
    return retorno;
}

int crear_semaforo(key_t key, int size, int *semid){
    int retorno;
    short *initial = NULL;

    if(!semid){
        return ERROR;
    }
    initial = calloc(size, sizeof(short));
    if(!initial){
        return ERROR;
    }

    retorno = semget(key, size, IPC_CREAT);
    *semid = retorno;
    retorno = inicializar_semaforo(retorno, initial);
    (retorno == -1) ? retorno = ERROR : OK;
    free(initial);
    return retorno;
}

int down_semaforo(int semid, int num_sem, int undo){
    int retorno;
    struct sembuf sops[1];
    sops[0].sem_num = num_sem;
    sops[0].sem_op = -1;
    sops[0].sem_flg = undo;
    retorno = semop(semid, sops, 1);
    (retorno == -1) ? retorno = ERROR : OK;
    return retorno;
}

int downMultiple_semaforo(int semid, int size, int undo, int *active){
    int retorno, i;
    struct sembuf *sops = NULL;
    if(!active){
        return ERROR;
    }
    sops = calloc(size, sizeof(struct sembuf));
    if(!sops){
        return ERROR;
    }
    for (i = 0; i < size; ++i) {
        sops[i].sem_num = active[i];
        sops[i].sem_op = -1;
        sops[i].sem_flg = undo;
    }
    retorno = semop(semid, sops, size);
    (retorno == -1) ? retorno = ERROR : OK;
    free(sops);
    return retorno;
}

int up_semaforo(int semid, int num_sem, int undo){
    int retorno;
    struct sembuf sops[1];
    sops[0].sem_num = num_sem;
    sops[0].sem_op = 1;
    sops[0].sem_flg = undo;
    retorno = semop(semid, sops, 1);
    (retorno == -1) ? retorno = ERROR : OK;
    return retorno;
}

int upMultiple_semaforo(int semid, int size, int undo, int *active){
    int retorno, i;
    struct sembuf *sops = NULL;
    if(!active){
        return ERROR;
    }
    sops = calloc(size, sizeof(struct sembuf));
    if(!sops){
        return ERROR;
    }
    for (i = 0; i < size; ++i) {
        sops[i].sem_num = active[i];
        sops[i].sem_op = 1;
        sops[i].sem_flg = undo;
    }
    retorno = semop(semid, sops, size);
    (retorno == -1) ? retorno = ERROR : OK;
    free(sops);
    return retorno;
}
