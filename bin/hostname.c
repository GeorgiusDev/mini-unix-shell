#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXIMUM 256

int main(int argc, char *argv []){
    char host[MAXIMUM];

    if(gethostname(host,sizeof(host))==-1){
        perror("hostname");
        exit(1);
    }

    printf("%s\n",host);
}