#include <stdio.h>
#include <stdlib.h>
#define STRING_LEN 80
int main (){
    void* structure;
    int pid;
    structure = calloc(1, (STRING_LEN+1)*sizeof(char) + sizeof(int));
    if (!(pid = fork())) {
        fgets((char *) structure, STRING_LEN, stdin);
        fwrite(structure, sizeof(char), STRING_LEN+1, stdout);
        free(structure);
        return EXIT_SUCCESS;
    }
    wait();
    fwrite(structure, sizeof(char), STRING_LEN+1, stdout);
    free(structure);
    return EXIT_SUCCESS;
}
