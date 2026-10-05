#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define MAXIMUM 1024

void operate(DIR *dir);
void options(DIR *dir,char option[],char path[]);

int main(int argc,char *argv[]){

	DIR *dir;

	if (argc == 1){
		dir = opendir(".");
		
		if(dir == NULL){
			perror("ls");
			return 1;
		}

		operate(dir);
	}
	
	
	else if (argc == 2){
		
		if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-a")==0){
			
			dir=opendir(".");
				
			if(dir == NULL){
				
				perror("ls");
				return 1;
			}
				
			options(dir,argv[1],".");
		}
		
		else{
		
			dir=opendir(argv[1]);
				
			if(dir == NULL){
				perror("ls");
				return 1;
			}
				
			operate(dir);
			
		}
	}
	else if (argc == 3){
		dir=opendir(argv[2]);
		
		if(dir == NULL){
			perror("ls");
			return 1;
		}
		
		options(dir,argv[1],argv[2]);
	}
	else{
		printf("ls: invalid arguments!\n");
		return 1;
	}
	
	
	
	closedir(dir);
	
	return 0;
}

void operate(DIR *dir){

	struct dirent *entry;
	
	printf("Files: ");
	
	while((entry = readdir(dir)) != NULL){
		if(entry->d_name[0] == '.'){
			continue;
		}
		if(isatty(STDOUT_FILENO)) printf("%s ",entry->d_name);
		else printf("%s\n",entry->d_name);
	}

	if(isatty(STDOUT_FILENO)) printf("\n");
	
}

void options(DIR *dir,char option[],char path[]){
	
	struct dirent *entry;
	struct stat info;
	
	if(strcmp(option,"-l")==0){
		char date [100];
		char type[20];
		
		char filepath[MAXIMUM];
		
		printf("%-20s %-10s %10s %s\n\n","Name:","Type:","Size:","Date modified:");
		
		while((entry = readdir(dir)) != NULL){
			if(entry->d_name[0] == '.'){
				continue;
			}
			strcpy(filepath,path);
			strcat(filepath,"/");
			strcat(filepath,entry->d_name);
			
			if(lstat(filepath, &info)==-1){
				perror("ls");
				continue;	
			}
			
			
			
			if(S_ISREG(info.st_mode)) strcpy(type,"File");
			else if(S_ISDIR(info.st_mode)) strcpy(type,"Directory");
			else if (S_ISLNK(info.st_mode)) strcpy(type,"Symbolic link");
			else strcpy(type,"Other");
			
			struct tm *timeinfo = localtime(&info.st_mtime);
			
			
			strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", timeinfo);
			
			printf("%-20s %-10s %10ld %s\n",
			entry->d_name,
			type,
			info.st_size,
			date);
			
		}
	}
	else if(strcmp(option,"-a") == 0){
		printf("Files: ");
		while((entry = readdir(dir)) != NULL)

			if(isatty(STDOUT_FILENO)) printf("%s ",entry->d_name);
			else printf("%s\n",entry->d_name);
			
		if(isatty(STDOUT_FILENO)) printf("\n");
	}
	else{
		printf("ls: unknown command!\n");
	}
	
}





