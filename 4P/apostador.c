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

Apostador *apos_set_pid(Apostador *a, pid_t pid){
    if(!a){
        return NULL;
    }
    a->pid = pid;
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

double apos_get_ben(Apostador *a){
    if(!a){
        return APOS_ERROR;
    }
    return a->ben;
}

Apostador *apos_incr_din_rest(Apostador *a, double delta){
    if(!a){
        return NULL;
    }
    a->din_rest+=delta;
    return a;
}

int apos_cmp_ben(const void *v1, const void *v2){
    Apostador *a1 = (Apostador *) v1;
    Apostador *a2 = (Apostador *) v2;
    if(!a1 || !a2){
        return 0;
    }
    return -1*(a1->ben - a2->ben);
}
