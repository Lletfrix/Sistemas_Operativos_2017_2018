/**
 * @brief Implementación de las primitivas del TAD caballo
 *
 * Este fichero contiene el código fuente del TAD caballo
 * @file caballo.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 09-05-2018
 */
#include <stdlib.h>

#include "caballo.h"

Caballo *cab_new(){
    return calloc(1, sizeof(Caballo));
}

void cab_destroy(Caballo *c){
    free(c);
}

Caballo *cab_set_id(Caballo *c, unsigned short id){
    if(!c){
        return NULL;
    }
    c->id = id;
    return c;
}

Caballo *cab_set_cot(Caballo *c, double cotizacion){
    if(!c){
        return NULL;
    }
    c->cotizacion = cotizacion;
    return c;
}

Caballo *cab_set_pos(Caballo *c, unsigned int posicion){
    if(!c){
        return NULL;
    }
    c->posicion = posicion;
    return c;
}

Caballo *cab_set_pid(Caballo *c, pid_t pid){
    if(!c){
        return NULL;
    }
    c->pid = pid;
    return c;
}

Caballo *cab_incr_apostado(Caballo *c, double delta){
    if(!c){
        return NULL;
    }
    c->dinero_apostado += delta;
    return c;
}

Caballo *cab_set_last_tir(Caballo *c, unsigned short tir){
    if(!c){
        return NULL;
    }
    c->last_tir = tir;
    return c;
}

unsigned short cab_get_last_tir(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->last_tir;
}

unsigned short cab_get_id(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->id;
}

unsigned int cab_get_pos(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->posicion;
}

pid_t cab_get_pid(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->pid;
}

double cab_get_cot(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->cotizacion;
}

double cab_get_apostado(Caballo *c){
    if(!c){
        return CAB_ERROR;
    }
    return c->dinero_apostado;
}
