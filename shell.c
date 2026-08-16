#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "commands.h"
#include "shell.h"

void cmdHandler(int n,char **tokens, char bin[]){

	if(isInternal(n,tokens)==1){
		
		pid_t pid = fork();
		int status;
		
		char path[MAXIMUM];
		
		strcpy(path,bin);
		strcat(path,"/");
		strcat(path,tokens[0]);
		
		if (pid<0){
			perror("fork");
			return;
		}
		else if(pid==0){
		
			external(n,tokens,path);

			outside(n,tokens);
			
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
				return;
			}
			else{
				perror("status");
			}
		}
		
	}
}

int isInternal(int n,char **tokens){

	int i,ncmd;

	command commands[] = {

	{"echo",echo},
	{"pwd",handle_pwd},
	{"cd",cd},
	
	{"mkdir",mdir},
	{"rmdir",rdir},
	
	{"touch",touch},
	{"rm",rm}

	};
	
	ncmd = sizeof(commands)/sizeof(commands[0]);
	
	for(i=0;i<ncmd;i++){
		if(strcmp(tokens[0],commands[i].name) == 0){
			commands[i].function(n,tokens);
			return 0;
		}
	}
	
	return 1;
	
}

int tokenise(char *line,char ***tokens, int *n){
	char *token;
	int i=0;
	
	token = strtok(line," \t");
	
	*tokens = malloc( (i+1) * sizeof(**tokens));
	
	if(*tokens == NULL){ 
		perror("memory");
		*n = 0;
		return -1;
	}
	
	(*tokens)[i] = token;
	
	while(token != NULL){
		char **tmp;
		token=strtok(NULL," \t");
		i++;
		tmp = realloc(*tokens,(i+1) * sizeof(**tokens));
		if(tmp == NULL){ 
			perror("memory");
			free(*tokens);
			*tokens = NULL;
			*n=0;
			return -1;
		}
		*tokens = tmp;
		(*tokens)[i] = token;
	}
	
	*n = i;
	return 0;
}







