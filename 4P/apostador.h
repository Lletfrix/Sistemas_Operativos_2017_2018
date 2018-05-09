#ifndef APOSTADOR
#define APOSTADOR

#define MAX_APOS_NAME 20
#define MAX_CAB 10
#define APOS_ERROR -1

typedef struct _Apostador{
    pid_t pid;
    char nombre[MAX_APOS_NAME];
    double ben[MAX_CAB];
    double total;
    double din_rest;
} Apostador;

Apostador *apos_new();

void apos_destroy(Apostador *);

Apostador *apos_init(Apostador *, char *, double);

Apostador *apos_set_ben(Apostador *, double, int);

Apostador *apos_set_pid(Apostador *, pid_t);

Apostador *apos_incr_din_rest(Apostador *, double);

size_t apos_sizeof();

char *apos_get_name(Apostador *);

double apos_get_ben(Apostador *, int);

pid_t apos_get_pid(Apostador *);

double apos_get_total(Apostador *);

double apos_get_din_rest(Apostador *);

int apos_cmp_ben(const void *, const void *);

Apostador *apos_refresh_total(Apostador *a);

#endif
