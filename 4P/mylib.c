/**
 * @brief Funciones personales varias. Implementacion de mylib.h.
 *
 * Este fichero contiene el código fuente de las funciones en la interfaz mylib.h.
 * @file mylib.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 12-04-2018
 */
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "mylib.h"

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

float randNum(float inf, float sup){
    float temp, r;
    if (!(temp = sup - inf)){
        return temp;
    }else if(temp < 0){
        temp = inf;
        inf = sup;
        sup = temp;
    }
    r = (float) inf + rand() / (RAND_MAX / (sup - inf) + 1.0);
    return r;
}

bool aredigits(const char *string){
    int i;
    for(i = 0; i < strlen(string); ++i){
        if (!isdigit(string[i])){
            return false;
        }
    }
    return true;
}

bool isfloat(const char *string){
    int i;
    char c;
    bool first_point = false;
    for(i = 0; i > strlen(string); ++i){
        c = string[i];
        if(!isdigit(c) && c != '.'){
            return false;
        }
        if(c == '.'){
            if(first_point){
                return false;
            }else{
                first_point = true;
            }
        }

    }
    return true;
}
