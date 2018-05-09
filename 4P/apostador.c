/**
 * @brief Implementación de las primitivas del TAD apostador
 *
 * Este fichero contiene el código fuente del TAD apostador
 * @file apostador.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "apostador.h"
#include "sim_carr_lib.h"

Apostador *apos_new(){
    return calloc(1, sizeof(Apostador));
}

void apos_destroy(Apostador *a){
    free(a);
}

Apostador *apos_init(Apostador *a, char *name, double din_init){
    int i;
    if(!a || !name || strlen(name)-1  > MAX_APOS_NAME){
        return NULL;
    }
    strcpy(a->nombre, name);
    a->din_rest = din_init;
    for (i = 0; i < 10; ++i) {
        a->ben[i] = 0.0;
    }
    return a;
}

Apostador *apos_set_ben(Apostador *a, double beneficio, int id){
    if(!a){
        return NULL;
    }
    a->ben[id] = beneficio;
    return a;
}

Apostador *apos_set_pid(Apostador *a, pid_t pid){
    if(!a){
        return NULL;
    }
    a->pid = pid;
    return a;
}

char *apos_get_name(Apostador *a){
    if(!a){
        return NULL;
    }
    return a->nombre;
}

pid_t apos_get_pid(Apostador *a){
    if(!a){
        return APOS_ERROR;
    }
    return a->pid;
}

double apos_get_ben(Apostador *a, int id){
    if(!a){
        return APOS_ERROR;
    }
    return a->ben[id];
}

Apostador *apos_incr_din_rest(Apostador *a, double delta){
    if(!a){
        return NULL;
    }
    a->din_rest+=delta;
    return a;
}

Apostador *apos_refresh_total(Apostador *a){
    int i;
    if(!a){
        return NULL;
    }
    for (i = 0, a->total = 0; i < n_cab; ++i){
        a->total += a->ben[i];
    }
    return a;
}

double apos_get_total(Apostador *a){
    if(!a){
        return APOS_ERROR;
    }
    return a->total;
}

int apos_cmp_ben(const void *v1, const void *v2){
    Apostador *a1 = (Apostador *) v1;
    Apostador *a2 = (Apostador *) v2;
    double total1, total2;
    if(!a1 || !a2){
        return 0;
    }
    return -1*(a1->total - a2->total);
}
