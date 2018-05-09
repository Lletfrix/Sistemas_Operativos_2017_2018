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

#define MAX_APOS_NAME 20
#define MAX_CAB 10
#define APOS_ERROR -1

typedef struct _Apostador{
    pid_t pid;
    char nombre[MAX_APOS_NAME];
    double ben[MAX_CAB];
    double total;
    double din_rest;
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
 * @param Apostador que queremos eliminar
 */
void apos_destroy(Apostador *);

/**
 * @brief Inicializa los datos de un apostador
 *
 * Da nombre y dinero inicial al apostador dado
 *
 * @param Apostador al que queremos poner los datos
 * @param String que representa el nombre del apostador
 * @param Dinero inicial del apostador
 * @return Apostador inicializado
 */
Apostador *apos_init(Apostador *, char *, double);

/**
 * @brief Setter del beneficio del apostador para un cierto caballo
 *
 * Nos permite establecer el beneficio generado por un cierto caballo al apostador
 *
 * @param Apostador al que queremos establecer el beneficio
 * @param Beneficio del apostador
 * @param Id del caballo del que proviene el beneficio
 * @return Apostador al que le hemos establecido el beneficio de un caballo
 */
Apostador *apos_set_ben(Apostador *, double, int);

/**
 * @brief Setter del pid de un apostador
 *
 * Nos permite establecer el pid a un apostador
 *
 * @param Apostador al que queremos establecer el pid
 * @param Pid del apostador
 * @return Apostador al que le hemos establecido el pid
 */
Apostador *apos_set_pid(Apostador *, pid_t);

/**
 * @brief Incrementa el dinero disponible del apostador
 *
 * Incrementa el dinero del apostador una cierta cantidad
 *
 * @param Apostador al que incrementar el dinero disponible
 * @param Dinero que incrementar
 * @return Apostador al que le hemos incrementado el dinero
 */
Apostador *apos_incr_din_rest(Apostador *, double);

/**
 * @brief Devuelve el tamaño de la estructura de apostador
 *
 * Devuelve el tamaño de la estructura de apostador en bytes
 *
 * @return Tamaño de la estructura
 */
size_t apos_sizeof();

/**
 * @brief Getter del nombre del apostador
 *
 * Devuelve el nombre del apostador
 *
 * @param Apostador del que queremos saber el nombre
 * @return Nombre del apostador
 */
char *apos_get_name(Apostador *);

/**
 * @brief Getter del beneficio de un apostador para cierto caballo
 *
 * Devuelve el beneficio de un apostador para cierto caballo
 *
 * @param Apostador del que queremos saber el beneficio
 * @param Id del caballo
 * @return Beneficio del apostador para ese caballo
 */
double apos_get_ben(Apostador *, int);

/**
 * @brief Getter del pid de un apostador
 *
 * Devuelve el pid del apostador
 *
 * @param Apostador del que queremos saber el pid
 * @return Pid del apostador
 */
pid_t apos_get_pid(Apostador *);

/**
 * @brief Getter de los beneficios totales del apostador
 *
 * Devuelve los beneficios totales del apostador
 *
 * @param Apostador del que queremos saber los beneficios
 * @return Beneficio total del apostador
 */
double apos_get_total(Apostador *);

/**
 * @brief Getter del dinero restante del apostador
 *
 * Devuelve el dinero disponible del apostador
 *
 * @param Apostador del que queremos saber el dinero restante
 * @return Dinero restante
 */
double apos_get_din_rest(Apostador *);

/**
 * @brief Compara los beneficios totales de dos apostadores
 *
 * Devuelve la diferencia entre los beneficios totales de dos apostadores
 *
 * @param Primer apostador
 * @param Segundo apostador
 * @return Resta del beneficio del segundo con el beneficio del primero
 */
int apos_cmp_ben(const void *, const void *);

/**
 * @brief Refresca los beneficios totales de un apostador
 *
 * Actualiza los beneficios totales de un apostador mirando sus beneficios por caballo
 *
 * @param Apostador al que queremos actualizar los beneficios
 * @return Apostador al que hemos actualizado los beneficios
 */
Apostador *apos_refresh_total(Apostador *a);

#endif
