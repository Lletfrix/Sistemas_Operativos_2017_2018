#define SIGSTART SIGUSR1
#define SIGTHROW SIGUSR2
#define SIGFREE SIGABRT
#define KEY_APUES_Q 1300
#define KEY_TIR_Q 1400
#define KEY_CAB_SHM 1500
#define KEY_APOS_SHM 1600
#define KEY_APUES_SHM 1700
#define READ 0
#define WRITE 1
#define NORMAL 0
#define GANADORA 1
#define REMONTAR 2
#define PATH "/bin/bash"


struct msgtir{
    long mtype; /*!< Tipo del mensaje */
    unsigned short tirada; /*!< Resultado de la tirada */
};
