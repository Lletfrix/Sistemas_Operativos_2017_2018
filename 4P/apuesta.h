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

typedef struct _Apuesta Apuesta; /*!< Define el tipo de datos Apuesta como la estructura de la misma*/

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
 * @param a Apuesta a eliminar
 */
void apuesta_destroy(Apuesta *a);

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
 * @brief Ejecuta una apuesta
 *
 * Ejecuta la apuesta a. Actualiza la cotizacion de los caballos, el estado del
 * apostador y escribe en un fichero dado por filepath el registro de la apuesta
 *
 * @param a Apuesta a ejecutar
 * @param filepath Ruta del fichero donde guardar las apuestas
 */
void apuesta_execute(Apuesta *a, char *filepath);

#endif
