/**
 * @brief Funciones comunes al ejercicio 13
 *
 * Este fichero contiene la implementación de las funciones comunes al ejercicio 13.
 * @file ejercicio13lib.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */

#define BUFF_SIZE 512 /*!< Tamaño del buffer */
#define NUM_THREADS 2 /*!< Numero de hilos */

typedef struct _datosMult{
    int **matriz;
    int escalar;
    int size;
    int id;
    int *fila_actual[NUM_THREADS];
};


void libera_matriz (int **matriz,unsigned int filas){
    int i;
    if(!matriz) return;

    for( i=0; i<filas; i++ ){
        free(matriz[i]);
    }
    free(matriz);
}

int **reserva_matriz(unsigned int size){
    int **m1, i;
    m1 = (int **) calloc(size, sizeof(int *));
    if(!m1) {
        return NULL;
    }
    for (i=0;i<size;i++) {
        m1[i] = (int *) calloc(size, sizeof(int));
        if (!m1[i]) {
            libera_matriz(m1, i);
            return NULL;
        }
    }
    return m1;
}

int **rellena_matriz (int** m1, char *buffer, unsigned int size){
    int i, j;
    char *token;
    token = strtok(buffer, " ");
    i = j = 0;
    while (token) {
        m1[i][j] = atoi(token);
        token = strtok(NULL, " ");
        ++j;
        if (!(j%size)){
            ++i;
            j = 0;
        }
    }
    return m1;
}

datosMult *datosMult_crear(int **m1, int n1, int size, int id){
    datosMult *datos1;
    datos1 = calloc(1, sizeof(datosMult));
    if(!datos1){
        return NULL;
    }
    datos1->matriz = m1;
    datos1->escalar = n1;
    datos1->size = size;
    datos1->id = id;
    return datos1;
}
