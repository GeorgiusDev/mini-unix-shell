#ifndef COMMANDS_H
#define COMMANDS_H

#define PATH_MAX 1024

void echo(int argc,char*argv[]);
char * pwd(char cwd[]);
void cd(int argc, char *argv[]);

void ls(int argc, char*argv[],char bin[]);

#endif