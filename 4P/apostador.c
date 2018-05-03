#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "apostador.h"

#define MAX_NAME 10

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
    return a;
}

Apostador *apos_set_ben(Apostador *a, double beneficio){
    if(!a){
        return NULL;
    }
    a->ben = beneficio;
    return a;
}

char *apos_get_name(Apostador *a){
    if(!a){
        return NULL;
    }
    return a->nombre;
}
