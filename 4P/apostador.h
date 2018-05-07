#ifndef APOSTADOR
#define APOSTADOR

#define MAX_APOS_NAME 10
#define APOS_ERROR -1

typedef struct _Apostador{
    pid_t pid;
    char nombre[MAX_APOS_NAME];
    double cant_apos;
    double ben;
    double din_rest;
} Apostador;

Apostador *apos_new();

void apos_destroy(Apostador *);

Apostador *apos_init(Apostador *, char *, double);

Apostador *apos_set_ben(Apostador *, double);

Apostador *apos_set_pid(Apostador *, pid_t);

Apostador *apos_incr_din_rest(Apostador *, double);

size_t apos_sizeof();

char *apos_get_name(Apostador *);

double apos_get_ben(Apostador *);

pid_t apos_get_pid(Apostador *);

double apos_get_din_rest(Apostador *);

int apos_cmp_ben(const void *, const void *);

#endif
