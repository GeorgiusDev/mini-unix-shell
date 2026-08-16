#ifndef SHELL_H
#define SHELL_H

typedef struct command{
	char *name;
	void(*function)(int,char*[]);

}command;

void cmdHandler(int n, char ** tokens, char bin[]);

int isInternal(int n, char **tokens);
int tokenise(char *line,char ***tokens, int *n);

#endif