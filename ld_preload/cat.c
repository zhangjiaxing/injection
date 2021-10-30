#include <stdio.h>

int main(){
    char buf[1025];

    FILE *fp = fopen("./Makefile", "r");
    if(fp != NULL){
        while(feof(fp) == 0){
            size_t nread = fread(buf, 1, 1024, fp);
            buf[nread] = '\0';
            printf("%s", buf);
        }
    }
}

