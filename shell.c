#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>

#include "commands.h"
#include "shell.h"

void cmdHandler(int n,char **tokens, char bin[]){
	
	int i,j;
	
	int isPiped = 0;
	
	/* check if there is "|", if there isn't then skip that chunk of a code */
	
	for(i=0;i<n;i++){
		if(strcmp(tokens[i],"|")==0){
			isPiped = 1;
			break;
		}
	}
	
	if(isPiped == 0){
		handleInternalExternal(n,tokens,bin);
		return;
	}
		
	/* for handling the loop */
	
	int isFirst = 1;
	
	int tagCount = 0;
	
	int cmdCount = 0;
	
	int wordIndex = 0;
	
	int tagsIndaRow = 0;
	
	/* for handling multiple commands */
	
	char ***arrayOfCommands = NULL;
	char *** reallocSignal;
	char ** wordSignal;
	
	int *words=NULL;
	int *wordsMem;
	
	if(strcmp(tokens[0],"|")==0){
		printf("shell: syntax error near unexpected token '|'\n");
		return;
	}
	
	/*Pipe handler*/
	
	for(i=0;i<n;i++){
	
		if(strcmp(tokens[i],"|")==0){
			tagsIndaRow++;
			
			/* check for the syntax error in pipes */
			
			if(tagsIndaRow>1){
				printf("shell: syntax error near unexpected token '|'\n");
				free(words);
				freeCommands(&arrayOfCommands,cmdCount);
				return;
			}
			tagCount++;
			
			/* NULL HANDLER */
			
			wordSignal = realloc(arrayOfCommands[cmdCount-1],(wordIndex+1)*sizeof(*arrayOfCommands[cmdCount-1]));
			
			if(wordSignal == NULL){
				freeCommands(&arrayOfCommands,cmdCount);
				free(words);
				perror("realloc");
				return;
			
			}
			arrayOfCommands[cmdCount-1] = wordSignal;
			
			
			arrayOfCommands[cmdCount-1][wordIndex] = NULL; 
			
			/*                     */
			
			words[cmdCount-1]=wordIndex;
			isFirst = 1;
			wordIndex=0;
			continue;
		}
		
		
		if(isFirst){
			tagsIndaRow = 0;
			
			isFirst = 0;
			
			reallocSignal = realloc(arrayOfCommands, (cmdCount+1) * sizeof(*arrayOfCommands));
			
			if(reallocSignal == NULL){
				perror("realloc");
				free(words);
				freeCommands(&arrayOfCommands,cmdCount);
				return;	
			}
			arrayOfCommands = reallocSignal;
			
			wordsMem = realloc(words,(cmdCount+1)*sizeof(*words));
			if(wordsMem == NULL){
				perror("realloc");
				free(words);
				freeCommands(&arrayOfCommands,cmdCount);
				return;
			}
			
			words=wordsMem;
			
			
			cmdCount++;
			arrayOfCommands[cmdCount-1] = NULL;
		}
		
		wordSignal = realloc(arrayOfCommands[cmdCount-1],(wordIndex+1)*sizeof(*tokens));
		
		if(wordSignal == NULL){
			freeCommands(&arrayOfCommands,cmdCount);
			free(words);
			perror("realloc");
			return;
		
		}
		arrayOfCommands[cmdCount-1] = wordSignal;
		
		arrayOfCommands[cmdCount-1][wordIndex] = tokens[i]; 
		
		wordIndex++;
		
	}
	words[cmdCount-1]=wordIndex;
	
	/* one last NULL adder */
	
		wordSignal = realloc(arrayOfCommands[cmdCount-1],(wordIndex+1)*sizeof(*tokens));
			
		if(wordSignal == NULL){
			freeCommands(&arrayOfCommands,cmdCount);
			free(words);
			perror("realloc");
			return;
			
		}
		
		arrayOfCommands[cmdCount-1] = wordSignal;
			
			
		arrayOfCommands[cmdCount-1][wordIndex] = NULL;
	
	/* CHECK IF PIPABLE */
	
	if(tagCount>=cmdCount){
		printf("shell: syntax error near unexpected token '|'\n");
		freeCommands(&arrayOfCommands,cmdCount);
		free(words);
		return;
	}
	
	/* END OF THIS CODE CHUNK */
	
	/* PIPE HANDLER */
	int fd[2];
	
	int prev_input = STDIN_FILENO;
	pid_t *pid;
	pid=malloc(cmdCount*sizeof(*pid));
	if(pid == NULL){
		freeCommands(&arrayOfCommands,cmdCount);
		free(words);
		return;
	}
	
	for(i=0;i<cmdCount;i++){
	
		if(i<(cmdCount-1)){
			if(pipe(fd)<0){
				perror("pipe");
				
				if(prev_input != STDIN_FILENO){
					close(prev_input);
				}
				break;
			}
		}
	
		pid[i] = fork();
		
		if(pid[i]<0){
			perror("fork");
			if(prev_input != STDIN_FILENO){

				close(prev_input);
				
			}
			if(i<(cmdCount-1)){
				close(fd[0]);
				close(fd[1]);
			}
			
			break;
		}
		if(pid[i]==0){
			if(i>0){
				if(dup2(prev_input,STDIN_FILENO)==-1){
					perror("dup2");
					exit(1);
				}
				close(prev_input);
			}
			if(i<(cmdCount-1)){
				close(fd[0]);
				if(dup2(fd[1], STDOUT_FILENO)==-1){
					perror("dup2");
					exit(1);
				}
				close(fd[1]);
			}
			
			if(checkInternal(words[i],arrayOfCommands[i])==1){
			
				char path[MAXIMUM];
		
				strcpy(path,bin);
				strcat(path,"/");
				strcat(path,arrayOfCommands[i][0]);
			
				external(words[i],arrayOfCommands[i],path);

				outside(words[i],arrayOfCommands[i]);
			
				perror("execv");
				exit(1);
			}
			exit(0);
		}
		else{
			if(i>0){
				close(prev_input);
			}
			if(i < cmdCount-1){
				close(fd[1]);
				prev_input = fd[0];
			}
		}
	}
	
	for(j=0;j<i;j++){
		if(waitpid(pid[j],NULL,0)==-1){
			perror("waitpid");
		}
	}
	
	/* END OF NEW CHUNK */
	
	freeCommands(&arrayOfCommands,cmdCount);
	free(words);
	free(pid);

}

void freeCommands(char****arrayOfCommands,int cmdCount){

	int j;
	
	for(j = 0; j<cmdCount;j++){
		free((*arrayOfCommands)[j]);
	}
	
	free(*arrayOfCommands);
	*arrayOfCommands = NULL;
	
}

int checkInternal(int n,char **tokens){

	int i,ncmd; /* i is loop counter, and ncmd is number of commands */
	
	/* these are built in commands */
	
	command commands[] = {

	{"echo",echo},
	{"pwd",handle_pwd},
	{"cd",cd},
	
	{"mkdir",mdir},
	{"rmdir",rdir},
	
	{"touch",touch},
	{"rm",rm},
	{"nscmd",nscmd}

	};
	
	ncmd = sizeof(commands)/sizeof(commands[0]); /* Get number of commands */
	
	/* Go throught each command, and compare strings, if found return 0, if not, return 1 */
	for(i=0;i<ncmd;i++){
		if(strcmp(tokens[0],commands[i].name) == 0){
			commands[i].function(n,tokens);
			return 0;
		}
	}
	
	return 1;
	
}

int tokenise(char *line,char***tokens,int *n){
	char *tmpToken;
	
	char **tmp;
	int i;
	
	*tokens = NULL;
	
	*n = 0;
	
	/* BOOLS for characters inside " ", or non spaces! */
	int insideWord=0;
	int insideStr=0;
	
	int isFirst=1;
	
	/* COUNTER */
	
	int letCount=0;
	
	/* Go from first charracter untill last (ignore the '\0') */
	
	for(i=0;line[i]!='\0';i++){
		
		/* If we are on space AND not inside string, and "insideWord" is true, then we no longer are inside the word! */
		
		if((line[i] == ' ' || line[i] == '\n' || line[i] == '\t') && !insideStr){
			
			if(insideWord){ 
				insideWord = 0;
				/* reset letter counter */
				letCount=0;
			}
			continue;
		}
		
		else if(line[i] == '"'){
			if(insideStr){
				insideStr = 0;
				
				if(line[i-1] == '"') continue; /* In case someone writes '""' it doesn't crash the program! */
				
				
				/* if next letter is space */
				
				if((line[i+1] == ' ' || line[i+1] == '\t' || line[i+1] == '\n' || line[i+1] == '\0')){
				
					/* Add "\0" to the end of this token, so str functions can work properly */
					letCount++;
					tmpToken=realloc((*tokens)[(*n)-1],letCount*sizeof(***tokens));
					if(tmpToken == NULL){
						perror("realloc");
						tokenFreer(tokens,n);
						
						return -1;
					}
					(*tokens)[(*n)-1]=tmpToken;
					(*tokens)[(*n)-1][letCount-1] = '\0';
					
				}
				
			}
			else{
				insideStr = 1;
			}
			continue;
		}
		
		/* If we are on non space letter, and "insideWord" is false, then we are inside the new word! */
		
		else{
			if(!insideWord) {
				insideWord = 1;
				isFirst=1;
			}
			else {
				if(isFirst) isFirst = 0;
			}
		}
		
		/* If we are on the first charracter of the token */
		
		if(isFirst){
			
			
			/* Allocate memory for that token */
			
			tmp = realloc(*tokens,((*n)+1) * sizeof(**tokens));
			if(tmp == NULL){
				perror("realloc");
				tokenFreer(tokens,n);
				return -1;
			}
			*tokens=tmp;
			(*n)++;
			(*tokens)[(*n)-1] = NULL;
		}
		
		/* if we are inside the word */
		if(insideWord){
		
			letCount++;
			/* Allocate memory for each letter inside the token */
			
			tmpToken=realloc((*tokens)[(*n)-1],letCount*sizeof(***tokens));
			if(tmpToken == NULL){
				perror("realloc");
				tokenFreer(tokens,n);
				return -1;
			}
			(*tokens)[(*n)-1]=tmpToken;
			(*tokens)[(*n)-1][letCount-1] = line[i]; /* Put line[i] as the letter of token */
			
			/* If next letter is space and we aren't inside string */
			
			if((line[i+1] == ' ' || line[i+1] == '\t' || line[i+1] == '\n' || line[i+1] == '\0') && (!insideStr || line[i+1] == '\0')){
			
				/* Add "\0" to the end of this token, so str functions can work properly */
				letCount++;
				tmpToken=realloc((*tokens)[(*n)-1],letCount*sizeof(***tokens));
				if(tmpToken == NULL){
					perror("realloc");
					tokenFreer(tokens,n);
					
					return -1;
				}
				(*tokens)[(*n)-1]=tmpToken;
				(*tokens)[(*n)-1][letCount-1] = '\0';
			}
		}
	}
	
	/* adding NULL for execv */
	
	tmp = realloc(*tokens,((*n)+1) * sizeof(**tokens));
	if(tmp == NULL){
		perror("realloc");
		tokenFreer(tokens,n);
		return -1;
	}
	*tokens=tmp;
	(*tokens)[*n]= NULL;
	
	return 0;
}
/* memory freeing */
void tokenFreer(char ***tokens,int *n){
	int i;	
	if(*tokens == NULL){ 
		*n = 0;
		return;
	}
	
	for(i=0;i<*n;i++){
		if((*tokens)[i]==NULL) continue;
		free((*tokens)[i]);
		(*tokens)[i] = NULL;
	}
	*n = 0;
	
	free(*tokens);
	*tokens = NULL;
}

/* OLD TOKENISER IF YOU PREFER SIMPLIFIED VERSION
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

*/

void handleInternalExternal(int n, char ** tokens,char bin[]){
	/* checks if the provided command is built-in or not */
	if(checkInternal(n,tokens)==1){
		
		/* create new process */
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

			/* if command is not in this program's /bin file, then it searches your device */

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

void getInfo(char ** username, char * hostname){

	/* get hostname */

    if(gethostname(hostname,256)==-1){
        perror("hostname");
        exit(1);
    }

	/* get username */

	uid_t uid = getuid();

    struct passwd *pw = getpwuid(uid);

    if(pw == NULL){
        perror("whoami");
        exit(1);
   }
   *username = pw->pw_name;
}


