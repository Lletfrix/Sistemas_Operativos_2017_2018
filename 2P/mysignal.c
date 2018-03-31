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
