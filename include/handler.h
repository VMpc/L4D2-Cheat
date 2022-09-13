/* hander.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _HANDLER_H
#define _HANDLER_H

#include "game.h"

typedef void (*cmdfunc)(Game *, char **restrict);
typedef struct {
  const char *name;
  cmdfunc func;
} Command;

typedef void (*togglefunc)(Game *, char);
typedef struct {
  int Key;
  char Value;
  togglefunc func;
} Toggle;

void executeCommand(Game *, char *restrict);

char **splitArguments(char[]);

#endif /* _HANDLER_H */
