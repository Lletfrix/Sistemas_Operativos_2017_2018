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

#define CAB_ERROR -1; /*!< Codigo de error del caballo*/

/**
 * @brief Informacion del caballo
 *
 * Estructura que contiene la informacion del caballo
 */
typedef struct _Caballo{
    pid_t pid; /*!< Pid del caballo*/
    unsigned short id; /*!< Id del caballo*/
    unsigned short last_tir; /*!< Ultima tirada del caballo*/
    double cotizacion; /*!< Cotización actual del caballo*/
    unsigned int posicion; /*!< Posicion actual del caballo*/
    double dinero_apostado; /*!< Dinero apostado al caballo*/
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

/**
 * @brief Setter de la posición del caballo
 *
 * Establece la posición del caballo
 *
 * @param Caballo a cambiar la posicion
 * @param Posición del caballo
 * @return Caballo al que hemos cambiado la posición
 */
Caballo *cab_set_pos(Caballo *, unsigned int);

/**
 * @brief Setter del id del caballo
 *
 * Establece el id del caballo
 *
 * @param Caballo a cambiar el id
 * @param Id del caballo
 * @return Caballo al que hemos cambiado el id
 */
Caballo *cab_set_id(Caballo *, unsigned short);

/**
 * @brief Setter del pid del caballo
 *
 * Establece el pid del caballo
 *
 * @param Caballo a cambiar el pid
 * @param Pid del caballo
 * @return Caballo al que hemos cambiado el id
 */
Caballo *cab_set_pid(Caballo *, pid_t);

/**
 * @brief Setter de la ultima tirada del caballo
 *
 * Establece la ultima tirada del caballo
 *
 * @param Caballo a cambiar la tirada
 * @param Valor de la ultima tirada del caballo
 * @return Caballo al que hemos cambiado la ultima tirada
 */
Caballo *cab_set_last_tir(Caballo *, unsigned short);

/**
 * @brief Incrementa el dinero apostado al caballo
 *
 * Incrementa el dinero apostado al caballo
 *
 * @param Caballo a modificar el dinero apostado
 * @param Incremento a añadir
 * @return Caballo al que hemos cambiado el dinero apostado
 */
Caballo *cab_incr_apostado(Caballo *, double);

/**
 * @brief Getter del id del caballo
 *
 * Devuelve el id del caballo
 *
 * @param Caballo del que se devuelve el id
 * @return Id del caballo
 */
unsigned short cab_get_id(Caballo *);

/**
 * @brief Getter de la ultima tirada del caballo
 *
 * Devuelve la ultima tirada del caballo
 *
 * @param Caballo del que se devuelve la ultima tirada
 * @return Ultima tirada del caballo
 */
unsigned short cab_get_last_tir(Caballo *);

/**
 * @brief Getter de la posicion del caballo
 *
 * Devuelve la posicion del caballo
 *
 * @param Caballo del que se devuelve la posicion
 * @return Posicion del caballo
 */
unsigned int cab_get_pos(Caballo *);

/**
 * @brief Getter del pid del caballo
 *
 * Devuelve el pid del caballo
 *
 * @param Caballo del que se devuelve el pid
 * @return Pid del caballo
 */
pid_t cab_get_pid(Caballo *);

/**
 * @brief Getter de la cotizacion del caballo
 *
 * Devuelve la cotizacion del caballo
 *
 * @param Caballo del que se devuelve la cotizacion
 * @return Cotizacion del caballo
 */
double cab_get_cot(Caballo *);

/**
 * @brief Getter del dinero apostado del caballo
 *
 * Devuelve el dinero apostado del caballo
 *
 * @param Caballo del que se devuelve el dinero apostado
 * @return Dinero apostado
 */
double cab_get_apostado(Caballo *);

#endif
