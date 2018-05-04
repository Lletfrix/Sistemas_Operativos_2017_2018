#define SIGSTART SIGUSR1
#define SIGTHROW SIGUSR2
#define KEY_APUES_Q 1300
#define KEY_TIR_Q 1310
#define KEY_CAB_SHM 1400
#define KEY_APOS_SHM 1410
#define KEY_APUES_SHM 1420
#define KEY_SEM_MON 1500
#define KEY_SEM_TUR 1510
#define READ 0
#define WRITE 1
#define NORMAL 0
#define GANADORA 1
#define REMONTAR 2
#define PATH "/bin/bash"
#define MAX_NAME 20
#define RUTA_FICHERO_APUESTAS "txt/apuestas.txt"

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
