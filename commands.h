#ifndef COMMANDS_H
#define COMMANDS_H

#define MAXIMUM 1024

/* Built in */

void echo(int argc,char*argv[]);

char * pwd(char cwd[]);
void cd(int argc, char *argv[]);

void mdir(int argc, char *argv[]);
void rdir(int argc, char * argv[]);

void touch(int argc, char*argv[]);
void rm(int argc, char*argv[]);

/*	External */

void ls(int argc, char*argv[],char bin[]);

#endif