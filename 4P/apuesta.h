#ifndef APUESTA
#define APUESTA

double apuesta_total = 0;

typedef struct _apuesta Apuesta;

Apuesta *apuesta_new();

void apuesta_destroy(Apuesta *);

Apuesta *apuesta_init(Apuesta *a, Apostador *apos, Caballo *c, unsigned short ventanilla, double cantidad);

void apuesta_execute(FILE* fp, Apuesta *a);

#endif
