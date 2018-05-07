#define SIGSTART SIGUSR1
#define SIGTHROW SIGUSR2
#define KEY_APUES_Q 1300
#define KEY_TIR_Q 1310
#define KEY_APUES_SHM 1400
#define KEY_CAB_SHM 1410
#define KEY_APOS_SHM 1430
#define KEY_GEN_SHM 1430
#define KEY_MON_SEM 1500
#define KEY_TUR_SEM 1510
#define KEY_CAB_SEM 1520
#define KEY_GEN_SEM 1530
#define SHM_ERROR (void *) -1
#define READ 0
#define WRITE 1
#define NORMAL 0
#define GANADORA 1
#define REMONTAR 2
#define PATH "/bin/bash"
#define MAX_NAME 10
#define RUTA_FICHERO_APUESTAS "txt/apuestas.txt"

extern int num_proc, n_cab, n_apos, n_vent;
extern double din;

struct msgtir{
    long mtype; /*!< Tipo del mensaje */
    unsigned short tirada; /*!< Resultado de la tirada */
};

struct msgapues{
    long mtype; /*!< Tipo de mensaje*/
    char nombre[MAX_NAME]; /*!< Información del mensaje*/
    int caballo;
    double cantidad;
};
