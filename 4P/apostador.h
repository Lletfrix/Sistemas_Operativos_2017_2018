#ifndef APOSTADOR
#define APOSTADOR

typedef struct _apostador{
    pid_t pid;
    char nombre[MAX_NAME];
    double cant_apos;
    double ben;
    double din_rest;
} Apostador;

Apostador *apos_new();

void apos_destroy(Apostador *);

Apostador *apos_init(Apostador *, char *, double);

Apostador *apos_set_ben(Apostador *, double);

Apostador *apos_set_pid(Apostador *, pid_t);

char *apos_get_name(Apostador *);

pid_t apos_get_pid(Apostador *);

#endif
