#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int factorial(int a){
    int i, fact;
    fact = 1;
    for(i = 2; i <= a; i++){
      fact = fact * i;
    }
    return fact;
}

void main(int argc, char* argv[]){
    int i, pipe_status, cpid, res;
    int fd[8][2];
    int ops[2];
    char datos[4];
    char oper[4];
    char* mensj;
    char resultado[256];
    char* separaciones;
    if (argc != 3){
      fprintf(stderr, "Usage is ./ejercicio8 op1 op2");
      exit(1);
    }
    for (i = 0; i < 8; i++){
        pipe_status = pipe(fd[i]);
        if(pipe_status == -1){
            fprintf(stderr, "Error creating a pipe.\n");
            exit(1);
        }
    }
    sprintf(datos, "%d,%d", atoi(argv[0]), atoi(argv[1]));
    for (i = 0; i < 4; i++){
        if(cpid = fork() == -1){
            while(wait(NULL) != -1){
            }
            fprintf(stderr, "Error forking in fork nº%d", i);
            exit(1);
        }
        if(!cpid){
          for (i = 0; i < 4; i++){
              close(fd[i][0]);
              close(fd[i+4][1]);
              write(fd[i][1], datos, sizeof(datos));
          }
          while(wait(NULL) != -1){
          }
          for (i = 4; i < 8; i++){
              read(fd[i][0], resultado, sizeof(resultado));
              printf("%s\n", resultado);
          }
        }
        if(cpid){
            switch(i){
                case 0:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                separaciones = strtok(oper, ",");
                while (separaciones != NULL){
                  ops[i] = atoi(separaciones);
                  separaciones = strtok(NULL, ",");
                }
                res = pow(ops[0], ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Potencia: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                break;
                case 1:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                separaciones = strtok(oper, ",");
                while (separaciones != NULL){
                  ops[i] = atoi(separaciones);
                  separaciones = strtok(NULL, ",");
                }
                res = ops[0]/ops[1];
                res = factorial(res);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Factorial: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                break;
                case 2:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                separaciones = strtok(oper, ",");
                while (separaciones != NULL){
                  ops[i] = atoi(separaciones);
                  separaciones = strtok(NULL, ",");
                }
                res = pow(ops[0], ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Combinaciones: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                break;
                case 3:
                close(fd[i][1]);
                close(fd[i+4][0]);
                read(fd[i][0], oper, sizeof(oper));
                separaciones = strtok(oper, ",");
                while (separaciones != NULL){
                  ops[i] = atoi(separaciones);
                  separaciones = strtok(NULL, ",");
                }
                res = fabs(ops[0]) + fabs(ops[1]);
                sprintf(mensj, "Datos enviados a través de la tubería por el proceso PID = %d. Operando 1: %d. Operando 2: %d. Suma de absolutos: %d", getpid(), ops[0], ops[1], res);
                write(fd[i+4][1], mensj, sizeof(mensj));
                break;
                default:
                fprintf(stderr, "Unexpected value of i = %d", i);
                exit(1);
            }
        }
    }
}
