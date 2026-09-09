#ifndef SHELL_H
#define SHELL_H

typedef struct command{
	char *name;
	void(*function)(int,char*[]);

}command;

void cmdHandler(int n, char ** tokens, char bin[]);
void freeCommands(char****arrayOfCommands,int cmdCount);

int checkInternal(int n, char **tokens);
int tokenise(char *line,char ***tokens, int *n);
void tokenFreer(char ***tokens,int *n);
void handleInternalExternal(int n, char ** tokens,char bin[]);

#endif