/**
 * @brief TAD Caballo con su estructura de datos y primitivas
 *
 * Este fichero contiene la estructura y funciones necesarias para simular el
 * comportamiento de un caballo.
 *
 * @file caballo.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#ifndef CABALLO
#define CABALLO

#define CAB_ERROR -1;

typedef struct _Caballo{
    pid_t pid;
    unsigned short id;
    unsigned short last_tir;
    double cotizacion;
    unsigned int posicion;
    double dinero_apostado;
} Caballo;

/**
 * @brief Constructor de un caballo
 *
 * Crea un nuevo caballo
 * @return Caballo generado
 */
Caballo *cab_new();

/**
 * @brief Elimina un caballo
 *
 * Libera la memoria asignada al caballo dado
 *
 * @param Caballo que queremos eliminar
 */
void cab_destroy(Caballo *);

/**
 * @brief Setter de la cotización del caballo
 *
 * Establece la cotización del caballo
 *
 * @param Caballo a cambiar la cotización
 * @param Cotización del caballo
 * @return Caballo al que hemos cambiado la cotización
 */
Caballo *cab_set_cot(Caballo *, double);

Caballo *cab_set_pos(Caballo *, unsigned int);

Caballo *cab_set_id(Caballo *, unsigned short);

Caballo *cab_set_pid(Caballo *, pid_t);

Caballo *cab_set_last_tir(Caballo *, unsigned short);

Caballo *cab_incr_apostado(Caballo *, double);

size_t cab_sizeof();

unsigned short cab_get_id(Caballo *);

unsigned short cab_get_last_tir(Caballo *);

unsigned int cab_get_pos(Caballo *);

pid_t cab_get_pid(Caballo *);

double cab_get_cot(Caballo *);

double cab_get_apostado(Caballo *);

Caballo **cab_get_shm_arr(Caballo *, size_t);

#endif
