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

/* Checks & runs a command with the split arguments */
void executeCommand(Game *restrict, char *restrict);
/* Handle key presses to make hotkeys */
void handleInput(Game *restrict, int);
/* Split stdin into a command + arguments */
char **splitArguments(char *restrict);

#endif /* _HANDLER_H */
