#include <stdlib.h>

#include "caballo.h"

struct _caballo{
    unsigned short id;
    double cotizacion;
    unsigned int posicion;
    double dinero_apostado;
};

Caballo *_cab_set_id(Caballo *, unsigned short);

Caballo *cab_new(unsigned short id){
    return _cab_set_id(calloc(1, sizeof(Caballo)), id);
}

void cab_destroy(Caballo *c){
    free(c);
}

Caballo *_cab_set_id(Caballo *c, unsigned short id){
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

Caballo *cab_incr_apostado(Caballo *c, double delta){
    if(!c){
        return NULL;
    }
    c->dinero_apostado += delta;
    return c;
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
