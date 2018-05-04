#include <stdlib.h>
#include <stdio.h>

#include "apostador.h"
#include "caballo.h"
#include "apuesta.h"


struct _Apuesta{
    Apostador *apos;
    Caballo* c;
    unsigned short ventanilla;
    double cantidad;
};

double apuesta_total = 0;


void _apuesta_print(FILE *, Apuesta *, double);

Apuesta *apuesta_new(){
    return calloc(1, sizeof(Apuesta));
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

void apuesta_execute(Apuesta *a, char *path){
    //TODO: Control de errores, Proteger la memoria del caballo (y del apostador?)
    FILE *fp;
    double old_cot;
    fp = fopen(path, "a");
    apos_set_ben(a->apos, cab_get_cot(a->c)*a->cantidad);
    apuesta_total += a->cantidad;
    //TODO: DOWN MUTEX CABALLO
    cab_incr_apostado(a->c, a->cantidad);
    cab_set_cot(a->c, apuesta_total/cab_get_apostado(a->c));
    //TODO: UP MUTEX CABALLO
    _apuesta_print(fp, a, old_cot);
    fclose(fp);
}

void _apuesta_print(FILE *fp, Apuesta *a, double old_cot){
    if(!fp || !a){
        return;
    }
    fprintf(fp, "Apostador: %s, Ventanilla: %hu, Caballo: %hu, Cotizacion: %lf, Cantidad %lf\n", apos_get_name(a->apos),a->ventanilla, cab_get_id(a->c), old_cot, a->cantidad);
}
