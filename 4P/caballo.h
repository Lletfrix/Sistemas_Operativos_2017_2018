#ifndef CABALLO
#define CABALLO

#define CAB_ERROR -1;

typedef struct _caballo{
    pid_t pid;
    unsigned short id;
    double cotizacion;
    unsigned int posicion;
    double dinero_apostado;
} Caballo;

Caballo *cab_new(unsigned short);

void cab_destroy(Caballo *);

Caballo *cab_set_cot(Caballo *, double);

Caballo *cab_set_pos(Caballo *, unsigned int);

Caballo *cab_set_id(Caballo *, unsigned short);

Caballo *cab_set_pid(Caballo *, pid_t);

Caballo *cab_incr_apostado(Caballo *, double);

unsigned short cab_get_id(Caballo *);

unsigned int cab_get_pos(Caballo *);

pid_t cab_get_pid(Caballo *)

double cab_get_cot(Caballo *);

double cab_get_apostado(Caballo *);

#endif