#ifndef COMMANDS_H
#define COMMANDS_H

#define MAXIMUM 1024

/* Built in */

void echo(int argc,char*argv[]);

void handle_pwd(int argc, char *argv[]);
char * pwd(char cwd[]);
void cd(int argc, char *argv[]);

void mdir(int argc, char *argv[]);
void rdir(int argc, char * argv[]);

void touch(int argc, char*argv[]);
void rm(int argc, char*argv[]);

/* nscmd searches the system for the specified command instead of using a builtin command or command from this project's /bin directory. */

void nscmd(int argc, char *argv[]);

/*	External */

void external(int argc, char*argv[],char bin[]);
void outside(int argc, char*argv[]);

#endif