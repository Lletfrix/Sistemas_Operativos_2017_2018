/**
 * @brief Interfaz de funciones personales acercas de señales.
 *
 * Este fichero contiene las funciones personales acerca de señales.
 *
 * @file mysignal.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 06-04-2018
 */
#ifndef MYSIGNAL
#define MYSIGNAL

/**
 * @brief Añade una lista de señales a un set.
 *
 * Añade una lista de señales acabada en -1 al set dado por sigt.
 * Ejemplo de uso : sigaddset_var(sigt, SIGUSR1, SIGUSR2, SIGINT, -1);
 *
 * @param sigt Puntero de tipo sigset_t. Set de señales.
 * @param sig Indices de las distinas señales. De extension variable. Debe terminar en -1.
 */
void sigaddset_var(sigset_t *sigt, int sig, ...);
#endif
