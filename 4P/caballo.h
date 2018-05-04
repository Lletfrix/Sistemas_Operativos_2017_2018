#ifndef CABALLO
#define CABALLO

#define CAB_ERROR -1;

typedef struct _Caballo Caballo;

Caballo *cab_new();

void cab_destroy(Caballo *);

Caballo *cab_set_cot(Caballo *, double);

Caballo *cab_set_pos(Caballo *, unsigned int);

Caballo *cab_set_id(Caballo *, unsigned short);

Caballo *cab_set_pid(Caballo *, pid_t);

Caballo *cab_set_last_tir(Caballo *, unsigned short);

Caballo *cab_incr_apostado(Caballo *, double);

size_t cab_sizeof();

unsigned short cab_get_id(Caballo *);

unsigned short cab_get_last_tir(Caballo *);

unsigned int cab_get_pos(Caballo *);

pid_t cab_get_pid(Caballo *);

double cab_get_cot(Caballo *);

double cab_get_apostado(Caballo *);

#endif
