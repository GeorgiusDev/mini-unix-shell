#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "commands.h"

char *pwd(char cwd[]){
	
	if(getcwd(cwd,MAXIMUM)!=NULL){
		return cwd;
	}
	else{
		perror("pwd");
		return " ";
	}
	
}

void handle_pwd(int argc, char *argv[]){
	char cwd[MAXIMUM];
	printf("Current path: %s\n",pwd(cwd));
}

void cd(int argc, char *argv[]){
	char cwd[MAXIMUM];

	if (argc == 1){
		printf("Change directory to where? Type 'cd PATH'! \n");
		return;
	}
	else if(argc>2){
		printf("Type only the directory!\n");
		return;
	
	}
	
	if(chdir(argv[1])==0){
		printf("Changed path to: %s\n",pwd(cwd));
	}
	else perror("cd");
}

void echo(int argc,char*argv[]){

	int i,len;
	
	for(i = 1; i<argc;i++){
		len = strlen(argv[i]);
		
		if(write(1,argv[i],len) == -1){
			perror("Echo");
			return;
		}
		if(write(1," ",1)==-1){
			perror("Echo");
			return;
	}
	}
	if(write(1,"\n",1)==-1){
		perror("Echo");
		return;
	}

}

void mdir(int argc, char * argv[]){

	if (argc == 1){
		printf("mkdir: enter the name or path of the directory you wish to create!\n");
		return;
	}
	else if (argc == 2){
		if(mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1){
			perror("mkdir");
		}
	}
	
	else{
		printf("mkdir: invalid arguments, type 'mkdir DIRECTORY' or 'mkdir PATH'!\n");
		return;
	}

}

void rdir(int argc, char * argv[]){
	
	if(argc == 1){
		printf("rmdir: enter the name or path of directory you wish to delete\n");
		return;
	}
	else if(argc == 2){
		if(rmdir(argv[1])==-1){
			perror("rmdir");
		}
	}
	else{
		printf("rmdir: invalid arguments, type 'rmdir DIRECTORY' or 'rmdir PATH'!\n");
	}
	
}

void touch(int argc, char*argv[]){
	int i,start = 1;
	int fd;
	
	if (argc == 1){
		printf("touch: invalid arguments, type 'touch filename.extension'\n");
		return;
	}
	if(argc == 2){
		if((fd=open(argv[1], O_CREAT | O_WRONLY,0666))== -1){
			perror("open");
			return;
		}
		close(fd);
	}	
	else{
		/* this will be used once I introduce the flags */
		if(argv[1][0] == '-'){
			start = 2;
		}
		
		for(i=start;i<argc;i++){
			if((fd=open(argv[i], O_CREAT | O_WRONLY,0666))== -1){
				perror("open");
				return;
			}
			close(fd);
			
		}
	}
	
}

void rm(int argc, char* argv[]){
	int i;
	if (argc == 1){
		printf("rm: type name of file/s you want to delete!\n");
		return;
	}	
	for(i=1;i<argc;i++){
		if(unlink(argv[i])==-1){
			perror("rm");
		}
	}
	
}

/* nscmd searches the system for the specified command instead of using a builtin command or command from this project's /bin directory. */

void nscmd(int argc, char *argv[]){
	int new_argc = argc-1,i;
	char **new_argv;

	if(new_argc<1){
		printf("nscmd: enter a command\n");
		return;
	}

	new_argv=malloc((new_argc + 1)*sizeof(*new_argv));

	if(new_argv == NULL){
		perror("malloc");
		return;
	}

	for(i=0;i<new_argc;i++){
		new_argv[i] = argv[i+1];
	}

	new_argv[new_argc] = NULL;

	pid_t pid = fork();
	int status;

	if(pid<0){
		perror("fork");
		free(new_argv);
		return;
	}
	else if(pid == 0){

		outside(new_argc,new_argv);
		exit(1);
	}
	else{
		if(waitpid(pid,&status,0)==-1){
			perror("waitpid");
			free(new_argv);

			return;
		}
		if(WIFEXITED(status)){
			free(new_argv);

			return;
		}
		else if(WIFSIGNALED(status)){
			printf("Process was killed!\n");
			free(new_argv);

			return;
		}
		else{
			perror("status");
			free(new_argv);

		}
	}
	
}

/* EXTERNAL */

void external(int argc, char *argv[],char bin[]){

	execv(bin,argv);
	return;
	
}

/* Outside the project */

void outside(int argc, char*argv[]){

	execvp(argv[0],argv);
	return;
}











