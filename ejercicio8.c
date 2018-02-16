#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXECL 0
#define EXECLP 1
#define EXECV 2
#define EXECVP 3

void usage(){
    printf("Usage is: ./ejercicio8 <program1> <program2> .... <-l|-lp|-v|-vp>\n");
    exit(1);
}

int main (int argc, char **argv){
    int exec_choice;
    char *strarr[2];
    char path[128];
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
    sprintf(path, "/bin/bash");
    if( !fork() ){
        for(i = 1; i < argc - 1; i++){
            if( !fork() ){
                switch (exec_choice) {
                    case EXECL:
                        //strcat(path, argv[i]);
                        execl(path, "bash", "-c", argv[i], NULL);
                        fprintf(stderr, "Error in execl, with arg %s\n", argv[i]);
                        exit(1);
                    case EXECLP:
                        execlp(argv[i], argv[i], NULL);
                        fprintf(stderr, "Error in execlp\n");
                        exit(1);
                    case EXECV:
                        strarr[0] = argv[i];
                        strarr[1] = NULL;
                        strcat(path, argv[i]);
                        execv(path, strarr);
                        fprintf(stderr, "Error in execv\n");
                        exit(1);
                    case EXECVP:
                        strarr[0] = argv[i];
                        strarr[1] = NULL;
                        fprintf(stderr, "Error in execvp\n");
                        execv(argv[i], strarr);
                        exit(1);
                    default:
                        fprintf(stderr, "%s\n", "Something went wrong");
                        exit(1);
                }
            }
            wait();
        }
    }else{
        wait();
    }
    exit(0);
}
