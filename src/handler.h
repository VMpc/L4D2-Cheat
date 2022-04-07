#ifndef _HANDLER_H
#define _HANDLER_H

#define MAX_COMMANDS 50

void addCommand(char *, int, char *(*func)(Game *, char **));

char *executeCommand(Game *, char **, int);

void splitArguments(Game *, char *);

typedef struct Cmd
{
  char *name;
  int argCount;
  char *(*func)(Game *, char **);
} Command;

#endif
