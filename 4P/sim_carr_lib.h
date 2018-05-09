/**
 * @brief Macros, variables globales y estructuras generales del proyecto.
 *
 * Este fichero contiene las macros, variables globales y estructuras necesarias
 * para el proyecto.
 *
 * @file sim_carr_lib.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */

#define SIGSTART SIGUSR1 /*!< Redefinicion de la señal SIGUSR1 que indica el comienzo de la carreara*/
#define SIGTHROW SIGUSR2 /*!< Redefinicion de la señal SIGUSR2 que hace que los hijos relaicen una tirada*/
#define KEY_APUES_Q 1300 /*!< Clave para generar la cola de apuestas*/
#define KEY_TIR_Q 1310 /*!< Clave para generar la cola de tiradas de los caballos*/
#define KEY_CAB_SHM 1400 /*!< Clave para generar la memoria compartida de los caballos*/
#define KEY_APOS_SHM 1410 /*!< Clave para generar la memoria compartida de los apostadores*/
#define KEY_MON_SEM 1500 /*!< Clave para generar el semaforo del monitor*/
#define KEY_TUR_SEM 1510 /*!< Clave para generar el semaforo que controla el turno de tirada*/
#define KEY_CAB_SEM 1520 /*!< Clave para generar el semaforo que controla el acceso a la memoria compartida de los caballos*/
#define KEY_GEN_SEM 1530 /*!< Clave para generar el semaforo general que hace esperar a cada proceso al comienzo hasta que el principal le da paso*/
#define READ 0 /*!< Opcion de lectura en la pipe*/
#define WRITE 1 /*!< Opcion de escritura en la pipe*/
#define NORMAL 0 /*!< Tirada normal del caballo*/
#define GANADORA 1 /*!< Tirada ganadora del caballo*/
#define REMONTAR 2 /*!< Tirada remontadora del caballo*/
#define PATH "/bin/bash" /*!< Path para generar las claves de distintos IPCS*/
#define MAX_NAME 20 /*!< Tamaño maximo del nombre del apostador en el mensaje*/
#define RUTA_FICHERO_APUESTAS "text/apuestas.txt" /*!< Ruta del fichero donde se guardan las apuestas*/
#define TIEMPO_PRE_CARR 30 /*!< Tiempo pre-carrera para apostar*/
#define MAX_APOS 100 /*!< Numero maximo de apostadores*/

extern int n_cab; /*!< Numero de caballos en la simulacion. Se define en sim_carr_lib.c y se inicializa una vez validados los parametros de entrada*/
extern int n_apos; /*!< Numero de apostadores en la simulacion. Se define en sim_carr_lib.c y se inicializa una vez validados los parametros de entrada*/
extern int n_vent; /*!< Numero de ventanillas en la simulacion. Se define en sim_carr_lib.c y se inicializa una vez validados los parametros de entrada*/
extern double din; /*!< Dinero inicial de los apostadores la simulacion. Se define en sim_carr_lib.c y se inicializa una vez validados los parametros de entrada*/
extern int num_proc; /*!< Numero de procesos hijo: n_cab + n_apos + 2 (Gestor y monitor). Se define en sim_carr_lib.c y se inicializa una vez validados los parametros de entrada*/

/**
* @brief Estructura del mensaje de tirada
*
* Esta estructura guarda la informacion del mensaje de tirada.
*/
struct msgtir{
    long mtype; /*!< Tipo del mensaje. Es igual al ID del caballo */
    unsigned short tirada; /*!< Resultado de la tirada */
};

/**
* @brief Informacion del mensaje de apuesta
*
* Esta estructura guarda la informacion del mensaje de apuesta.
*/
struct msgapues{
    long mtype; /*!< Tipo de mensaje. Es igual al ID del apostador*/
    char nombre[MAX_NAME]; /*!< Nombre del apostador*/
    int caballo; /*!< Id del caballo al que apostar*/
    double cantidad; /*!< Cantidad que se apostó*/
};
