/**
 * @brief Funciones comunes al ejercicio 2
 *
 * Este fichero contiene las funciones comunes al ejercicio 2.
 * @file ejercicio2_lib.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 15-04-2018
 */
#ifndef EJERCICIO2_LIB
#define EJERCICIO2_LIB

#define KEY1 1300 /*!< Numero para generar una key con ftok*/
#define KEY2 1400 /*!< Numero para generar una key con ftok*/
#define KEY3 1500 /*!< Numero para generar una key con ftok*/
#define PATH "/bin/bash" /*!< Path para generar una key con ftok*/
#define MAX_NOMBRE 80 /*!< Maxima longitud del nombre*/

/**
* @brief Informacion del usuario
*
* Esta estructura guarda la información de un usuario.
*/
typedef struct info {
    char nombre[MAX_NOMBRE]; /*!< Nombre del usuario*/
    int id; /*!< Id del usuario*/
} Info;

/**
 * @brief Imprime el uso correcto del ejercicio 2.
 *
 * Imprime el orden de los parametros de entrada de ambos ejercicios 2.
 */
void usage();

#endif
