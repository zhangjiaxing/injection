#define _GNU_SOURCE
#include <dlfcn.h>
#include <ctype.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct hook_node {
    char *name;
    void *func;
    struct list_head list;
};

struct hook_node hook_head;


static void* load_sym(const char *symname, void *proxyfunc) {

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

/*
#define SETUP_SYM(X) do { true_ ## X = load_sym( # X, X ); } while(0)
typedef size_t (*fread_t)(void*, size_t, size_t, FILE*);
fread_t true_fread = NULL;
*/

#define ORIGIN_SYM(X) ((typeof(&X)) get_sym(#X))
#define ORIGIN_CALL(X, ...) (ORIGIN_SYM(X)(__VA_ARGS__))
#define SETUP_SYM(X) do { setup_sym( #X, X); } while(0)

void *get_sym(const char *name){
    struct hook_node *cur;
    list_for_each_entry(cur, &hook_head.list, list){
        if(strcmp(name, cur->name) == 0){
            return cur->func;
        }
    }
    return NULL;
}


void setup_sym(const char *name, void *proxyfunc){
    struct hook_node *cur;
    list_for_each_entry(cur, &hook_head.list, list){
        if(strcmp(name, cur->name) == 0){
            fprintf(stderr, "setup_sym error %s\n", name);
            exit(0);
        }
    }

    struct hook_node *node = malloc(sizeof(*node));
    if(node == NULL){
        fprintf(stdout, "malloc error\n");
    }
    node->name = strdup(name);
    node->func = load_sym(name, proxyfunc);
    INIT_LIST_HEAD(&node->list);
    list_add_tail(&node->list, &hook_head.list);
}

void do_init(){
    static atomic_flag init_flag = ATOMIC_FLAG_INIT;
    if(! atomic_flag_test_and_set(&init_flag)){

        INIT_LIST_HEAD(&hook_head.list);
		SETUP_SYM(fread);
	}
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
	do_init();
	
	char *p = ptr;
	size_t nread = ORIGIN_CALL(fread, ptr, size, nmemb, stream);
	for(size_t i=0; i<nread; i++){
		if(isalpha(p[i])){
			p[i] = toupper(p[i]);
		}
	}
}

