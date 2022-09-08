/* hander.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _HANDLER_H
#define _HANDLER_H

#include "game.h"

#define MAX_COMMANDS 50

void addCommand(char *, int8_t, char *(*func)(Game *, char **));

char *executeCommand(Game *, char **, int8_t);

void splitArguments(Game *, char *);

typedef struct Cmd {
  char *name;
  int8_t argCount;
  char *(*func)(Game *, char **);
} Command;

#endif /* _HANDLER_H */
