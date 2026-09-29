#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

	int fd;
	int bytes;

	char buffer[128];
	
	/* Open the specified file and redirect stdin to it */
	if(argc==2){
		fd = open(argv[1],O_RDONLY);
		
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
	/* Edge case scenario handler */
	else if (argc>2){
		printf("For now, shell supports only 'CAT FILENAME'!\n");
		exit(0);
	
	}
	
	/* Read input in chunks and write each chunk to stdout.
	   This also allows cat to work with input from a pipe.
	*/
	
	while((bytes = read(0,buffer,sizeof(buffer)))>0){
		if(write(1,buffer,bytes) == -1){
			perror("write");
			exit(1);
		}
	}
	
	return 0;	

}







