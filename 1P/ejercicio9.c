/**
 * @brief Ejercicio 9
 *
 * Este fichero contiene el código fuente del ejercicio 9 de la entrega.
 * @file ejercicio9.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int factorial(unsigned int);
int binom_coef(unsigned int, unsigned int);
void leer_operandos(char* , char* , int*);

void main(int argc, char* argv[]){
    int i, j, pipe_status, cpid, res;
    int fd[8][2];
    int ops[2];
    char datos[4];
    char oper[256];
    char mensj[256];
    char resultado[256];
    char* separaciones;
    if (argc != 3){
      fprintf(stderr, "Usage is ./ejercicio9 op1 op2\n");
      exit(1);
    }
    for (i = 0; i < 8; i++){
        pipe_status = pipe(fd[i]);
        if(pipe_status == -1){
            fprintf(stderr, "Error creating a pipe.\n");
            exit(1);
        }
    }
    sprintf(datos, "%d,%d", atoi(argv[1]), atoi(argv[2]));
    for (i = 0; i < 4; i++){
        if((cpid = fork()) == -1){
            while(wait(NULL) != -1){
            }
            fprintf(stderr, "Error forking in fork nº%d", i);
            exit(1);
        }
        if(!cpid){
          switch(i){
            case 0:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                leer_operandos(oper, separaciones, ops);
                res = pow(ops[0], ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Potencia: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                exit(0);
            case 1:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                leer_operandos(oper, separaciones, ops);
                res = ops[0]/ops[1];
                res = factorial(res);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Factorial: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                exit(0);
            case 2:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                leer_operandos(oper, separaciones, ops);
                res = binom_coef(ops[0], ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Combinaciones: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                exit(0);
            case 3:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                leer_operandos(oper, separaciones, ops);
                res = fabs(ops[0]) + fabs(ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Suma de absolutos: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                exit(0);
            default:
                fprintf(stderr, "Unexpected value of i = %d", i);
                exit(1);
          }
        }
    }
    if(cpid){
      for (i = 0; i < 4; i++) {
        close(fd[i][0]);
        close(fd[i+4][1]);
        write(fd[i][1], datos, sizeof(datos));
      }
        while(wait(NULL) != -1);
        for (i = 0; i < 4; i++){
          read(fd[i+4][0], resultado, sizeof(resultado));
          printf("%s\n", resultado);
        }
    }
}

/**
 * @brief Factorial de un numero
 *
 * factorial() devuelve el valor del factorial de un numero
 * @param a numero entero no negativo
 * @return entero con valor factorial de a
 */
int factorial(unsigned int a){
    int i, fact;
    fact = 1;
    for(i = 2; i <= a; i++){
      fact = fact * i;
    }
    return fact;
}

/**
 * @brief Coeficiente binomial
 *
 * binom_coef() devuelve el valor del coeficiente binomial de a sobre b
 * @param a Elemento superior del coeficiente binomial
 * @param b Elemento inferior del coeficiente binomial
 * @return Valor del coeficiente binomial
 */
int binom_coef(unsigned int a, unsigned int b){
    if(b == 1 || b == a){
        return 1;
    }
    if(b > a){
        return 0;
    }
    return (factorial(a))/((factorial(b))*factorial(a - b));
}

/**
 * @brief Lee los operandos a partir de un buffer.
 *
 * Lee operandos a partir de una cadena de caracteres separadas por comas.
 * @param oper buffer inicial
 * @param separaciones buffer donde se almacenan los tokens de oper
 * @param ops array de operandos.
 */
void leer_operandos(char* oper, char* separaciones, int* ops){
    int j;
    separaciones = strtok(oper, ",");
    j = 0;
    while (separaciones != NULL){
      ops[j] = atoi(separaciones);
      separaciones = strtok(NULL, ",");
      j++;
    }
}
