#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "shell.h"


int tokenise(char *line, char***tokens,int *n);

int main(){
	
	char *line=NULL;
	size_t len = 0;
	int n;
	
	char bin[MAXIMUM];
	
	if(getcwd(bin,MAXIMUM)==NULL){
		perror("Directory");
		exit(1);
	}
	
	strcat(bin,"/bin");
	
	char **tokens=NULL;
	
	while(1){
		printf("$: ");
		if(getline(&line, &len, stdin) == -1){
			if(feof(stdin)) break;
			
			perror("getline");
			return 1;
		}
		line[strcspn(line,"\n")] = '\0';
		
		if(tokenise(line,&tokens,&n) == -1){
			printf("Tokenisation failed!\n");
			free(tokens);
			continue;
		}
		
		if(n == 0) {
			free(tokens);
			tokens = NULL;
			continue;
		
		}
		
		if(strcmp(tokens[0],"exit")==0) break;
		
		cmdHandler(n,tokens,bin);
		
		if(tokens != NULL){
			free(tokens);
			tokens = NULL;
		}
		

	}
	
	free(line);
	free(tokens);
	return 0;
}



