/* hander.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _HANDLER_H
#define _HANDLER_H

#include "game.h"

#define MAX_COMMANDS 50

typedef void (*cmdfunc)(Game *, char **restrict);

typedef struct {
  char *name;
  int8_t argCount;
  cmdfunc func;
} Command;

void addCommand(char *restrict, int8_t, cmdfunc);

void executeCommand(Game *, char **, int8_t);

void splitArguments(Game *, char *);

#endif /* _HANDLER_H */
