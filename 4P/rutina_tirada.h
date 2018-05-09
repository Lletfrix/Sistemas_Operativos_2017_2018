/**
 * @brief Fichero que contiene la cabecera de la rutina que sigue cada proceso tirada
 *
 * Este fichero contiene la cabecera de la rutina que sigue cada proceso de tiradas
 *
 * @file rutina_tirada.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#ifndef RUTINA_TIRADA
#define RUTINA_TIRADA

/**
 * @brief Rutina del proceso monitor
 *
 * Funcion que ejecuta el proceso monitor nada más haberse bifurcado
 *
 * @param id Id del caballo que realiza las tiradas
 * @param pipe Tuberia por la que se comunica el tipo de tirada al caballo.
 */
void proc_tirada(int id, int *pipe);

#endif
