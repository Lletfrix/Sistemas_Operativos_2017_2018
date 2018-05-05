#ifndef APUESTA
#define APUESTA

//TODO: Decide if this is necessary
extern double apuesta_total;

typedef struct _Apuesta Apuesta;

Apuesta *apuesta_new();

void apuesta_destroy(Apuesta *);

Apuesta *apuesta_init(Apuesta *a, Apostador *apos, Caballo *c, unsigned short ventanilla, double cantidad);

void apuesta_execute(Apuesta *a, char *filepath, int n_cab);

#endif
