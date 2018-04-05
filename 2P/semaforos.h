#ifndef SEMAFOROS
#define SEMAFOROS

#define OK EXIT_SUCCESS
#define ERROR -1
/***************************************************************
Nombre: Inicializar_Semaforo.
Descripcion: Inicializa los semaforos indicados.
Entrada:
 int semid: Identificador del semaforo.
 unsigned short *array: Valores iniciales.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
************************************************************/
int inicializar_semaforo(int semid, unsigned short *array);
/***************************************************************
Nombre: Borrar_Semaforo.
Descripcion: Borra un semaforo.
Entrada:
 int semid: Identificador del semaforo.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int borrar_semaforo(int semid);
/***************************************************************
Nombre: Crear_Semaforo.
Descripcion: Crea un semaforo con la clave y el tamaño
especificado. Lo inicializa a 0.
Entrada:
 key_t key: Clave precompartida del semaforo.
 int size: Tamaño del semaforo.
 int  *semid: Identificador del semaforo.
Salida:
 int: ERROR en caso de error,
 0 si ha creado el semaforo,
 1 si ya estaba creado.
**************************************************************/
int crear_semaforo(key_t key, int size, int *semid);
/**************************************************************
Nombre:Down_Semaforo
Descripcion: Baja el semaforo indicado
Entrada:
 int semid: Identificador del semaforo.
 int num_sem: Semaforo dentro del array.
 int undo: Flag de modo persistente pese a finalización
abrupta.
Salida:
 int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int down_semaforo(int semid, int num_sem, int undo);
/***************************************************************
Nombre: DownMultiple_Semaforo
Descripcion: Baja todos los semaforos del array indicado
 por active.
Entrada:
 int semid: Identificador del semaforo.
 int size: Numero de semaforos del array.
 int undo: Flag de modo persistente pese a finalización
abrupta.
 int *active: Semaforos involucrados.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int down_multiple_semaforo(int semid, int size, int undo, int *active);
/**************************************************************
Nombre:Up_Semaforo
Descripcion: Sube el semaforo indicado
Entrada:
 int semid: Identificador del semaforo.
 int num_sem: Semaforo dentro del array.
 int undo: Flag de modo persistente pese a finalizacion
 abupta.
Salida:
 int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int up_semaforo(int semid, int num_sem, int undo);
/***************************************************************
Nombre: UpMultiple_Semaforo
Descripcion: Sube todos los semaforos del array indicado
por active.
Entrada:
 int semid: Identificador del semaforo.
 int size: Numero de semaforos del array.
 int undo: Flag de modo persistente pese a finalización
abrupta.
 int *active: Semaforos involucrados.
Salida:
int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int up_multiple_semaforo(int semid, int size, int undo, int *active);

#endif
