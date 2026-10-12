#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "shell.h"
#include "colors.h"
#include "history.h"

int emptyHistoryBuffer(char *history[100], int n, char *bin);

int main(){
	int i;

	/* variables for tokenisation */
	char *line=NULL;
	size_t len = 0;
	int n;
	
	/* path variable for project and bin folder */

	char historypath[MAXIMUM];
	char bin[MAXIMUM];

	/* variables for history */

	char *history[100] = {NULL};
	int historyCounter = 0;

	/* get hostname, and username */

	char hostname[256];
	char *username;

	getInfo(&username, hostname);
	
	/* get this project's path and then add '/bin' and '/history' */

	if(getcwd(bin,MAXIMUM)==NULL){
		perror("Directory");
		exit(1);
	}

	strcat(bin,"/bin");

	strcpy(historypath,bin);

	strcat(historypath,"/history.txt");
	
	char **tokens=NULL;
	
	while(1){
		/* check if history buffer is full, and then empty it */
		if(historyCounter == 100){
			if(emptyHistoryBuffer(history,historyCounter,historypath)==0){
				historyCounter = 0;
			}
			else{
				printf("history: history error occured!\n");
				break;
			}
		}

		/* print the line text, and get input line */
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

		/* write command to history buffer*/

		*(history+historyCounter)=strdup(line);

		if(*(history+historyCounter) == NULL){
			perror("strdup");

			free(tokens);
			tokens=NULL;
			
			continue;
		}

		historyCounter++;

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

	/* history cleanup */
	
	if (historyCounter != 0){
		if(emptyHistoryBuffer(history,historyCounter,historypath)==1){
			printf("history: history error occured!\n");
			for(i=0; i < historyCounter; i++){
				free(*(history+i));
				*(history+i)=NULL;
			}
		}
	}
	
	/* memory cleanup */
	
	free(line);
	free(tokens);
	return 0;
}