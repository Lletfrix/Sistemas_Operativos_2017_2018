#include <stdlib.h>
#include <stdio.h>

#include "apuesta.h"
#include "apostador.h"
#include "caballo.h"

struct _apuesta{
    Apostador *apos;
    Caballo* c;
    unsigned short ventanilla;
    double cantidad;
};

void _apuesta_print(FILE *, Apuesta *);

Apuesta *apuesta_new(){
    return calloc(1, Apuesta *);
}

void apuesta_destroy(Apuesta *a){
    free(a);
}

Apuesta *apuesta_init(Apuesta *a, Apostador *apos, Caballo *c, unsigned short ventanilla, double cantidad){
    if(!a || !apos || !c){
        return NULL;
    }
    a->apos = apos;
    a->c = c;
    a->ventanilla = ventanilla;
    a->cantidad = cantidad;
    return a;
}

void apuesta_execute(FILE* fp, Apuesta *a){
    //TODO: Synchronise
    double old_cot;
    apos_set_ben(a->apos, cab_get_cot(a->c)*a->cantidad);
    apuesta_total += a->cantidad;
    cab_incr_apostado(a->c, a->cantidad);
    cab_set_cot(a->c, apuesta_total/cab_get_apostado(c));
    _apuesta_print(fp, a, old_cot);
}

void _apuesta_print(FILE *fp, Apuesta *a, double old_cot){
    if(!fp || !a){
        return;
    }
    fprintf(fp, "Apostador: %s, Ventanilla: %hu, Caballo: %hu, Cotizacion: %lf, Cantidad %lf\n", apos_get_name(a->apostador),a->ventanilla, cab_get_id(a->c), old_cot, a->cantidad);
}
