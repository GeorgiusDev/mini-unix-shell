#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "commands.h"

char *pwd(char cwd[]){
	
	if(getcwd(cwd,PATH_MAX)!=NULL){
		return cwd;
	}
	else{
		perror("pwd");
		return " ";
	}
	
}

void cd(int argc, char *argv[]){
	char cwd[PATH_MAX];

	if (argc == 1){
		printf("Change directory to where? Type 'cd PATH'! \n");
		return;
	}
	else if(argc>2){
		printf("Type only the directory!\n");
		return;
	
	}
	
	if(chdir(argv[1])==0){
		printf("Changed path to: %s\n",pwd(cwd));
	}
	else perror("cd");
}

void echo(int argc,char*argv[]){

	int i,len;
	
	for(i = 1; i<argc;i++){
		len = strlen(argv[i]);
		
		if(write(1,argv[i],len) == -1){
			perror("Echo");
			return;
		}
		if(write(1," ",1)==-1){
			perror("Echo");
			return;
	}
	}
	if(write(1,"\n",1)==-1){
		perror("Echo");
		return;
	}

}

void ls(int argc, char *argv[],char bin[]){

	pid_t pid = fork();
	int status;
	
	char path[PATH_MAX];
	
	strcpy(path,bin);
	strcat(path,"/ls");
	
	if (pid<0){
		perror("fork");
		return;
	}
	else if(pid==0){
		execv(path,argv);
		perror("execv");
		exit(1);
	}
	else{
		if(waitpid(pid,&status,0)==-1){
			perror("waitpid");
			return;
		}
		if(WIFEXITED(status)){
			return;
		}
		else if(WIFSIGNALED(status)){
			printf("Process was killed!\n");
		}
		else{
			perror("ls");
		}
	}
	
}











