#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXIMUM 1024

int main(int argc, char *argv[]){
	int i;
	
	int fd;
	int bytes;
	int strsize;
	char *word;
	
	if(argc == 3){
		fd = open(argv[2],O_RDONLY);
		
		if(fd == -1){
			perror("open");
			exit(1);
		}
		if(dup2(fd,0)== -1){
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);		
	}
	
	
	else if (argc == 1 || argc > 3){
		printf("Usage: grep PATTERNS [file] or cmd1 | grep PATTERNS\n");
		exit(0);
	}
	
	if (argv[1][0] == '-'){
		printf("Usage: grep PATTERNS [file] or cmd1 | grep PATTERNS\n");
		printf("Options are not supported\n");
		exit(0);
	}
	
	strsize = strlen(argv[1]);
	word = malloc((strsize+1) * sizeof(*word));
	
	if (word == NULL){
		perror("malloc");
		exit(1);
	}
		
	if(strcpy(word,argv[1])==NULL){
		free(word);
		perror("strcpy");
		exit(1);
	}
	
	/* GREP HANDLER */
	
	int line = 1;
	int line_pos = 0;
	
	char buffer[MAXIMUM];
	char line_buffer[MAXIMUM + 1];
	
	while((bytes = read(0,buffer,MAXIMUM))>0){
		
		for(i=0; i<bytes; i++){
			line_buffer[line_pos] = buffer[i];
			line_pos++;
			
			if(buffer[i] == '\n'){
				line_buffer[line_pos] = '\0';
				
				if(strstr(line_buffer,word)!=NULL){
				
					if(argc == 3) {
						printf("%d: ",line);
						fflush(stdout);
					}
					
					if(write(1,line_buffer,line_pos) == -1){
						perror("write");
						exit(1);
					}
				}
				
				line++;
				line_pos = 0;
				
			}
			
		}
		
	
	}
	
}
