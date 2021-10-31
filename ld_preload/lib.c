#include <stdatomic.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


atomic_flag init_flag = ATOMIC_FLAG_INIT;

static void* load_sym(char* symname, void* proxyfunc) {
    void *funcptr = dlsym(RTLD_NEXT, symname);
    if(!funcptr) {
        fprintf(stderr, "Cannot load symbol '%s' %s\n", symname, dlerror());
        exit(1);
    } else {
        fprintf(stderr, "loaded symbol '%s'" " real addr %p  wrapped addr %p\n", symname, funcptr, proxyfunc);
    }
    if(funcptr == proxyfunc) {
        fprintf(stderr, "circular reference detected, aborting!\n");
        abort();
    }
    return funcptr;
}

#define SETUP_SYM(X) do { true_ ## X = load_sym( # X, X ); } while(0)

typedef size_t (*fread_t)(void*, size_t, size_t, FILE*);
fread_t true_fread = NULL;

void do_init(){
    bool ret = atomic_flag_test_and_set(&init_flag);
    if(ret){
        SETUP_SYM(fread);
    }
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
    do_init();

    char *p = ptr;
    size_t nread = true_fread(ptr, size, nmemb, stream);
    for(size_t i=0; i<nread; i++){
        if(isalpha(p[i])){
            p[i] = toupper(p[i]);
        }
    }
}

