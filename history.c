#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

int emptyHistoryBuffer(char *history[100],int n,char *bin){
	int i;
	
	FILE *f = fopen(bin,"a");
	
	if(f == NULL){
		perror("fopen");
		return 1;
	}

	/* print to txt file */

	for(i=0;i<n;i++){
		if(fprintf(f,"%s\n",*(history+i))==-1){
			perror("fprintf");
			fclose(f);
			return 1;
		}
	
	}

	/* free memory */

	if(fclose(f) == EOF){
		perror("fclose");
		return 1;
	}

	for(i=0;i<n;i++){
		free(*(history+i));
		*(history+i) = NULL;
	}

	return 0;
}

void printHistory(char **history, int n, char *historyPath){
	int i,counter;
	char buffer[255];
	counter = 0;

	FILE *f = fopen(historyPath, "r");
	if (f == NULL){
		perror("fopen");
		return;
	}

	while(fgets(buffer,255,f) != NULL){
		counter++;
		printf("%d %s",counter,buffer);
	}

	if(fclose(f)==EOF){
		perror("fclose");
		return;
	}

	if(n>0){
		for(i=0; i<n; i++){
			counter++;
			printf("%d %s\n",counter,*(history+i));
		}
	}

}
