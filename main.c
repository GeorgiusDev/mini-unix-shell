#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "shell.h"
#include "colors.h"

int tokenise(char *line, char***tokens,int *n);

int main(){

	/* variables for tokenisation */
	char *line=NULL;
	size_t len = 0;
	int n;
	
	/* path variable for bin folder */

	char bin[MAXIMUM];

	/* get hostname, and username */

	char hostname[256];
	char *username;

	getInfo(&username, hostname);
	
	/* get this project's path and then add '/bin' */

	if(getcwd(bin,MAXIMUM)==NULL){
		perror("Directory");
		exit(1);
	}
	
	strcat(bin,"/bin");
	
	char **tokens=NULL;
	
	while(1){
		printf(COLOR_GREEN "%s@%s$: " COLOR_RESET,username,hostname);
		if(getline(&line, &len, stdin) == -1){
			if(feof(stdin)) break;
			
			perror("getline");
			return 1;
		}
		line[strcspn(line,"\n")] = '\0';

		/* break string into tokens */
		
		if(tokenise(line,&tokens,&n) == -1){
			printf("Tokenisation failed!\n");
			free(tokens);
			tokens = NULL;
			continue;
		}
		
		/* if it is empty line */

		if(n == 0) {
			free(tokens);
			tokens = NULL;
			continue;
		
		}

		/* exit handler */

		if(strcmp(tokens[0],"exit")==0) break;
		
		/* start the command */

		cmdHandler(n,tokens,bin);
		
		/* free the tokens to prevent leaks */

		if(tokens != NULL){
			free(tokens);
			tokens = NULL;
		}
		

	}

	/* memory cleanup */
	
	free(line);
	free(tokens);
	return 0;
}





