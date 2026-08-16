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
		
		/*OLD CODE, IN CASE OTHER FUNCTIONS EVER BREAK!*
		
		if(strcmp(tokens[0],"cwd") == 0 || strcmp(tokens[0],"pwd") == 0) handle_pwd(n,tokens);
		
		else if(strcmp(tokens[0],"cd") == 0) cd(n,tokens);
		
		else if(strcmp(tokens[0],"echo") == 0) echo(n,tokens);
		
		else if(strcmp(tokens[0],"mkdir") == 0) mdir(n,tokens);
		
		else if(strcmp(tokens[0],"rmdir") == 0) rdir(n,tokens);
		
		else if(strcmp(tokens[0],"touch")==0) touch(n,tokens);
		
		else if (strcmp(tokens[0],"rm")==0) rm(n,tokens);
		
		else if(strcmp(tokens[0],"break") == 0 || strcmp(tokens[0],"close") == 0 || strcmp(tokens[0],"exit")==0) break;
		
		*EXTERNAL*
		
		if(strcmp(tokens[0],"ls") == 0) ls(n,tokens,bin);
		
		else printf("shell: Unknown commfree(line);
	free(tokens);and!\n");
		
		free(tokens);
		tokens = NULL; */
	}
	
	
	
	free(line);
	free(tokens);
	return 0;
}



