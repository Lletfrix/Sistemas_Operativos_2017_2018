/**
 * @brief TAD Apostador con su estructura de datos y primitivas
 *
 * Este fichero contiene la estructura y funciones necesarias para simular el
 * comportamiento de un apostador.
 *
 * @file apostador.h
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#ifndef APOSTADOR
#define APOSTADOR

#define MAX_APOS_NAME 20 /*!< Tamaño maximo del nombre del apostador*/
#define MAX_CAB 10 /*!< Numero maximo de caballos*/
#define APOS_ERROR -1 /*!< Codigo de error del apostador*/

/**
 * @brief Estructura del apostador
 *
 * Estructura que contiene la informacion del apostador.
 */
typedef struct _Apostador{
    pid_t pid; /*!< Pid del apostador*/
    char nombre[MAX_APOS_NAME]; /*!< Nombre del apostador*/
    double ben[MAX_CAB]; /*!< Beneficios del apostador por cada caballo*/
    double total; /*!< Beneficios totales del apostador*/
    double din_rest; /*!< Dinero restante para apostar del apostador*/
} Apostador;

/**
 * @brief Constructor de un apostador
 *
 * Crea un nuevo apostador
 * @return Apostador generado
 */
Apostador *apos_new();

/**
 * @brief Elimina un apostador
 *
 * Libera la memoria asignada al apostador dado
 *
 * @param a Apostador que queremos eliminar
 */
void apos_destroy(Apostador *a);

/**
 * @brief Inicializa los datos de un apostador
 *
 * Da nombre y dinero inicial al apostador dado
 *
 * @param a Apostador al que queremos poner los datos
 * @param nombre String que representa el nombre del apostador
 * @param din Dinero inicial del apostador
 * @return Apostador inicializado
 */
Apostador *apos_init(Apostador *a, char *nombre, double din);

/**
 * @brief Setter del beneficio del apostador para un cierto caballo
 *
 * Nos permite establecer el beneficio generado por un cierto caballo al apostador
 *
 * @param a Apostador al que queremos establecer el beneficio
 * @param ben Beneficio del apostador
 * @param id Id del caballo del que proviene el beneficio
 * @return Apostador al que le hemos establecido el beneficio de un caballo
 */
Apostador *apos_set_ben(Apostador *a, double ben, int id);

/**
 * @brief Setter del pid de un apostador
 *
 * Nos permite establecer el pid a un apostador
 *
 * @param a Apostador al que queremos establecer el pid
 * @param id Pid del apostador
 * @return Apostador al que le hemos establecido el pid
 */
Apostador *apos_set_pid(Apostador *a, pid_t id);

/**
 * @brief Incrementa el dinero disponible del apostador
 *
 * Incrementa el dinero del apostador una cierta cantidad
 *
 * @param a Apostador al que incrementar el dinero disponible
 * @param din Dinero que incrementar
 * @return Apostador al que le hemos incrementado el dinero
 */
Apostador *apos_incr_din_rest(Apostador *a, double din);

/**
 * @brief Getter del nombre del apostador
 *
 * Devuelve el nombre del apostador
 *
 * @param a Apostador del que queremos saber el nombre
 * @return Nombre del apostador
 */
char *apos_get_name(Apostador *a);

/**
 * @brief Getter del beneficio de un apostador para cierto caballo
 *
 * Devuelve el beneficio de un apostador para cierto caballo
 *
 * @param a Apostador del que queremos saber el beneficio
 * @param id Id del caballo
 * @return Beneficio del apostador para ese caballo
 */
double apos_get_ben(Apostador *a, int id);

/**
 * @brief Getter del pid de un apostador
 *
 * Devuelve el pid del apostador
 *
 * @param a Apostador del que queremos saber el pid
 * @return Pid del apostador
 */
pid_t apos_get_pid(Apostador *a);

/**
 * @brief Getter de los beneficios totales del apostador
 *
 * Devuelve los beneficios totales del apostador
 *
 * @param a Apostador del que queremos saber los beneficios
 * @return Beneficio total del apostador
 */
double apos_get_total(Apostador *a);

/**
 * @brief Compara los beneficios totales de dos apostadores
 *
 * Devuelve la diferencia entre los beneficios totales de dos apostadores
 *
 * @param a1 Primer apostador
 * @param a2 Segundo apostador
 * @return Resta del beneficio del segundo con el beneficio del primero
 */
int apos_cmp_ben(const void *a1, const void *a2);

/**
 * @brief Refresca los beneficios totales de un apostador
 *
 * Actualiza los beneficios totales de un apostador mirando sus beneficios por caballo
 *
 * @param a Apostador al que queremos actualizar los beneficios
 * @return Apostador al que hemos actualizado los beneficios
 */
Apostador *apos_refresh_total(Apostador *a);

#endif
