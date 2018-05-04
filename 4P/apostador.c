#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "apostador.h"

struct _Apostador{
    pid_t pid;
    char nombre[MAX_APOS_NAME];
    double cant_apos;
    double ben;
    double din_rest;
};

Apostador *apos_new(){
    return calloc(1, sizeof(Apostador));
}

void apos_destroy(Apostador *a){
    free(a);
}

Apostador *apos_init(Apostador *a, char *name, double din_init){
    if(!a || !name || strlen(name) >= 10){
        return NULL;
    }
    strcpy(a->nombre, name);
    a->din_rest = din_init;
    a->cant_apos = 0.0;
    return a;
}

Apostador *apos_set_ben(Apostador *a, double beneficio){
    if(!a){
        return NULL;
    }
    a->ben = beneficio;
    return a;
}

size_t apos_sizeof(){
    return sizeof(Apostador);
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

double apos_get_din_rest(Apostador *a){
    if(!a){
        return APOS_ERROR;
    }
    return a->din_rest;
}


Apostador *apos_incr_din_rest(Apostador *a, double delta){
    if(!a){
        return NULL;
    }
    a->din_rest+=delta;
    return a;
}
