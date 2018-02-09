#include <stdio.h>
#include <stdlib.h>

#define EXECL 0
#define EXECLP 1
#define EXECV 2
#define EXECVP 3

void usage(){
    printf("Usage is: ./ejercicio8 <program1> <program2> .... <-l|-lp|-v|-vp>\n");
}

int main (int argc, char **argv){
    int exec_choice;
    if(argc <= 2){
        usage();
    }

    if(!strcmp(argv[argc-1], "-l")){
        exec_choice = EXECL;
    }else if(!strcmp(argv[argc-1], "-lp"){
        exec_choice = EXECLP;
    }else if(!strcmp(argv[argc-1], "-v"){
        exec_choice = EXECV;
    }else if(!strcmp(argv[argc-1], "-vp"){
        exec_choice = EXECVP;
    }else{
        printf("Argument number %d is wrong.\n", argc-1);
        usage();
    }
}
