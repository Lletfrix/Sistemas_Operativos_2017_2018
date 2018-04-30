#ifndef CABALLO
#define CABALLO

#define CAB_ERROR -1;

typedef struct _caballo Caballo;

Caballo *cab_new(unsigned short);

void cab_destroy(Caballo *);

Caballo *cab_set_cot(Caballo *, double);

Caballo *cab_set_pos(Caballo *, unsigned int);

Caballo *cab_incr_apostado(Caballo *, double);

unsigned short cab_get_id(Caballo *);

unsigned int cab_get_pos(Caballo *);

double cab_get_cot(Caballo *);

double cab_get_apostado(Caballo *);

#endif
