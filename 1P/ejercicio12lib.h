/**
 * @brief Funciones comunes al ejercicio 12
 *
 * Este fichero contiene las funciones comunes al ejercicio 12.
 * @file ejercicio12.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#ifndef EJERCICIO12
#define EJERCICIO12


#define NUM_PROCS 100 /*!< Constante que define el numero de procesos a ejecutar*/
#define NUM_THREADS 100 /*!< Constante que define el numero de hilos a ejecutar*/
#define SEC_TO_NSEC 1000000000 /*!< Constante de cambio entre segundos y nanosegundos */
#define DOUBLE_SEC_TO_NSEC 1000000000.0 /*!< Constante de cambio entre segundos y nanosegundos como float*/

/**
 * @brief Datos del hilo
 *
 * Esta estructura define los datos que usará el hilo, una string de
 * 100 caracteres y un entero n.
 */

 typedef struct _estructuraHilo {
     char string[100];
     int n; /*!< Numero de primos a calcular */
 }estructuraHilo;

/**
 * @brief Comprueba si un numero es primo
 *
 * es_primo() comprueba si un numero dado es primo
 * @param num entero al que comprobar si es primo o no
 * @return 1 si el número es primo, 0 en caso contrario
 */
int es_primo(unsigned int num);


/**
 * @brief Calcula un numero determinado de primos
 *
 * calcular_primos() calcula un numero de primos que recibe por la
 * entrada _estructuraHilo, que recibe como puntero a void
 * por el argumento de entrada.
 * @param void_data _estructuraHilo casteada a (void *)
 * @return NULL
 */
void *calcular_primos(void *void_data);

#endif
