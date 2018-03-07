/**
 * @brief Ejericio 8
 *
 * Este fichero contiene el código fuente del ejercicio 8 de la entrega.
 * @file ejercicio8.c
 * @author Rafael Sánchez & Sergio Galán
 * @version 1.0
 * @date 07-03-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXECL 0 /*!< Selección de ejecucion con execl */
#define EXECLP 1 /*!< Selección de ejecucion con execlp */
#define EXECV 2 /*!< Selección de ejecucion con execv */
#define EXECVP 3 /*!< Selección de ejecucion con execlvp */

/**
 * @brief Imprime el uso del programa
 *
 * usage() imprime en stdout el uso correcto del programa.
 */
void usage(){
    printf("Usage is: ./ejercicio8 <program1> <program2> .... <-l|-lp|-v|-vp>\n");
    exit(1);
}

int main (int argc, char **argv){
    int exec_choice;
    char *strarr[4];
    char bash_path[]={"/bin/bash"};
    char bash_arg[]={"-c"};
    char bash_name[]={"bash"};
    int i;
    if(argc <= 2){
        usage();
    }
    if(!strcmp(argv[argc-1], "-l")){
        exec_choice = EXECL;
    }else if(!strcmp(argv[argc-1], "-lp")){
        exec_choice = EXECLP;
    }else if(!strcmp(argv[argc-1], "-v")){
        exec_choice = EXECV;
    }else if(!strcmp(argv[argc-1], "-vp")){
        exec_choice = EXECVP;
    }else{
        printf("Argument number %d is wrong.\n", argc-1);
        usage();
    }
    if( !fork() ){
        for(i = 1; i < argc - 1; i++){
            if( !fork() ){
                switch (exec_choice) {
                    case EXECL:
                        //strcat(path, argv[i]);
                        execl(bash_path, bash_name, bash_arg, argv[i], NULL);
                        fprintf(stderr, "Error in execl, with arg %s\n", argv[i]);
                        exit(1);
                    case EXECLP:
                        execlp(argv[i], argv[i], NULL);
                        fprintf(stderr, "Error in execlp\n");
                        exit(1);
                    case EXECV:
                        strarr[0] = bash_path;
                        strarr[1] = bash_arg;
                        strarr[2] = argv[i];
                        strarr[3] = NULL;
                        execv(bash_path, strarr);
                        fprintf(stderr, "Error in execv\n");
                        exit(1);
                    case EXECVP:
                        strarr[0] = argv[i];
                        strarr[1] = NULL;
                        execvp(argv[i], strarr);
                        fprintf(stderr, "Error in execvp\n");
                        exit(1);
                    default:
                        fprintf(stderr, "%s\n", "Something went wrong");
                        exit(1);
                }
            }
            wait(NULL);
        }
    }else{
        wait(NULL);
    }
    exit(0);
}
