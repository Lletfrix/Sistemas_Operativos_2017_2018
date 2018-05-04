/**
 * @brief Interfaz de funciones personales varias.
 *
 * Este fichero contiene las funciones personales varias.
 *
 * @file mylib.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 12-04-2018
 */
#ifndef MYLIB
#define MYLIB

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

/**
 * @brief Genera un número real aleatorio en el rango [inf, sup)
 *
 * Genera un número real aleatorio en el rango [inf, sup)
 *
 * @param inf Límite inferior del intervalo
 * @param sup Límite superior del intervalo
 * @return Devuelve el número aleatorio generado
 */
float randNum(float inf, float sup);

/**
 * @brief Comprueba si una string es numerica.
 *
 * Comprueba si cada caracter de la cadena string es un valor numerico.
 *
 * @param string Cadena de caracteres.
 * @return Devuelve true si la cadena es numerica, false si no lo es.
 */
bool aredigits(const char *string);

/**
 * @brief Comprueba si una string es un float o entero.
 *
 * Comprueba si cada caracter de la cadena string es un valor numerico o un punto.
 *
 * @param string Cadena de caracteres.
 * @return Devuelve true si la cadena es un float o entero, false si no lo es.
 */
bool isfloat(const char *string);

#endif
