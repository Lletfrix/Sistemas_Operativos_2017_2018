/**
 * @brief Funciones personales acercas de señales. Implementacion de mysignal.h.
 *
 * Este fichero contiene el código fuente de las funciones en la interfaz mysignal.h.
 * @file mysignal.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 06-04-2018
 */
#include <signal.h>
#include <stdarg.h>

#include "mysignal.h"

void sigaddset_var(sigset_t *sigt, int sig, ...){
    va_list args;
    int i;

    va_start(args, sig);
    for (i = sig; i != -1; i = va_arg(args, int)) {
        sigaddset(sigt, i);
    }
    va_end(args);
    return;
}
