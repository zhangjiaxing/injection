#include <stdio.h>

int main(){
    char buf[1025];

    FILE *fp = fopen("./Makefile", "r");
    size_t nread = 1;
    if(fp != NULL){
        while(nread > 0){
            nread = fread(buf, 1, 1024, fp);
            buf[nread] = '\0';
            printf("%s", buf);
        }
    }
}

