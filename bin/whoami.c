#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

void main(int argc, char *argv[]){
    uid_t uid = getuid();

    struct passwd *pw = getpwuid(uid);

    if(pw == NULL){
        perror("whoami");
        exit(1);
   }

   printf("%s\n",pw->pw_name);

}