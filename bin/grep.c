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
	
	
	/* Open the specified file and redirect stdin to it */
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
	
	/* Edge-case scenarios handler */
	
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
	word = malloc((strsize+1) * sizeof(*word)); /* Allocate memory for PATTERN + '\0' */
	
	if (word == NULL){
		perror("malloc");
		exit(1);
	}
		
	strcpy(word,argv[1]); 

	
	/* GREP HANDLER */
	
	int line = 1;
	int line_pos = 0;
	
	char buffer[MAXIMUM];
	char line_buffer[MAXIMUM + 1];
	
	/* read() may return multiple lines or split a line between reads
	   therefore, bytes are accumulated in line_buffer untill '\n' is found.
	*/
	
	while((bytes = read(0,buffer,MAXIMUM))>0){ 
		
		for(i=0; i<bytes; i++){
			line_buffer[line_pos] = buffer[i];
			line_pos++;
			
			/* A complete line has been collected */
			if(buffer[i] == '\n'){
				line_buffer[line_pos] = '\0';
				
				/* Check if this line has the pattern */
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
				
				/* Move to the next line */
				line++;
				line_pos = 0;
				
			}
			
		}
		
	
	}
	
	free(word);
	
}
