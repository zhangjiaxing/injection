#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define err_exit(msg) do{ perror(msg); exit(-1); }while(0)

int main(int argc, char *argv[]){
    if(argc <= 1){
        fprintf(stderr, "usage: %s cat file\n", argv[0]);
        exit(0);
    }
    if(setenv("LD_PRELOAD", "./lib.so", 1) != 0){
        err_exit("setenv");
    }
    execvp(argv[1], &argv[1]);
    return -1;
}

