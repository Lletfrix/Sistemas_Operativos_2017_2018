/**
 * @brief Funciones comunes al ejercicio 13
 *
 * Este fichero contiene las funciones comunes al ejercicio 13.
 * @file ejercicio13.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#ifndef EjERCICIO13
#define EjERCICIO13

#define BUFF_SIZE 512 /*!< Tamaño del buffer */
#define NUM_THREADS 2 /*!< Numero de hilos */

/**

 * @brief Datos necesarios para la ejecucion de los hilos
 *
 * Esta estructura guarda los datos necesarios para la ejecucion
 * de los hilos, como la matriz, el escalar, el tamaño de la
 * matriz, y la id del hilo contrario.
 */
 typedef struct _datosMult{
     int **matriz;
     int escalar;
     int size;
     int id;
     int *fila_actual[NUM_THREADS];
 }datosMult;


/**
* @brief Libera la memoria de la matriz
*
* Libera la memoria reservada por reserva_matriz.
* @param matriz Doble puntero a entero que guarda la informacion de la matriz
* @param filas Numero de filas de la matriz
*/
void libera_matriz (int **matriz,unsigned int filas);
/**
* @brief reserva memoria para la matriz
*
* Reserva la memoria de la matriz
* @param size Tamaño de la matriz cuadrada
* @return Doble puntero a entero con la memoria para la matriz, inicializada a 0.
*/
int **reserva_matriz(unsigned int size);

/**
 * @brief Rellena la matriz con datos
 *
 * Dado un buffer, rellena la matriz con los datos de la cadena de
 * caracteres.
 * @param m1 Matriz con memoria asignada
 * @param buffer Cadena de caracteres con la informacion de la matriz
 * @param size Entero con el tamaño de la matriz
 */
int **rellena_matriz (int** m1, char *buffer, unsigned int size);

/**
 * @brief "Creador" de la estructura datosMult
 *
 * Asigma memoria para una estructura datosMult
 * @param m1 Matriz con memoria asignada
 * @param n1 Escalar
 * @param size Tamaño de la matriz
 * @param id Id del proceso
 */
datosMult *datosMult_crear(int **m1, int n1, int size, int id);

#endif
