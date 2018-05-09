/**
 * @brief TAD Apuesta con su estructura y primitivas
 *
 * Este fichero contiene la estructura y funciones necesarias para simular el
 * comportamiento de un apostador.
 *
 * @file apuesta.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#ifndef APUESTA
#define APUESTA

//TODO: Decide if this is necessary
extern double apuesta_total;

typedef struct _Apuesta Apuesta;

/**
 * @brief Constructor de una apuesta
 *
 * Crea una nueva apuesta, reservando memoria para ella
 *
 * @return Apuesta creada
 */
Apuesta *apuesta_new();

/**
 * @brief Destructor de una apuesta.
 *
 * Elimina una apuesta dada, liberando su memoria
 *
 * @param Apuesta a eliminar
 */
void apuesta_destroy(Apuesta *);

/**
 * @brief Inicializador de los valores de la apuesta
 *
 * Establece los valores iniciales de una apuesta dada
 *
 * @param a Apuesta a la que inicializar los valores
 * @param apos Apostador que ha generado la apuesta
 * @param c Caballo de la apuesta
 * @param cab_id Identificador del caballo de la apuesta
 * @param ventanilla Identificador de la ventanilla donde se gestionará la apuesta
 * @param cantidad Dinero apostado
 * @return Apuesta que hemos inicializado
 */
Apuesta *apuesta_init(Apuesta *a, Apostador *apos, Caballo *c, int cab_id, unsigned short ventanilla, double cantidad);

/**
 * @brief Añade una lista de señales a un set.
 *
 * Añade una lista de señales acabada en -1 al set dado por sigt.
 * Ejemplo de uso : sigaddset_var(sigt, SIGUSR1, SIGUSR2, SIGINT, -1);
 *
 * @param sigt Puntero de tipo sigset_t. Set de señales.
 * @param sig Indices de las distinas señales. De extension variable. Debe terminar en -1.
 */
void apuesta_execute(Apuesta *a, char *filepath);

#endif
