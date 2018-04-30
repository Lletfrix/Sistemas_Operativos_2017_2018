#ifndef APOSTADOR
#define APOSTADOR

typedef struct _apostador Apostador;

Apostador *apos_new();

void apos_destroy(Apostador *);

Apostador *apos_init(Apostador *, char *, double);

Apostador *apos_set_ben(Apostador *, double);

char *apos_get_name(Apostador *);

#endif
