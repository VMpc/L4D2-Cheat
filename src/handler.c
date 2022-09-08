/* keyboard.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/handler.h"
#include "../include/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmdAmount = -1;
struct Cmd Commands[MAX_COMMANDS];

/* Initializes a command for the handler */
void addCommand(char *cmdName, int8_t argCount, char *(*func)(Game *, char **)) {
  Command cmd;
  cmd.name = cmdName;
  cmd.argCount = argCount;
  cmd.func = func;
  Commands[cmdAmount + 1] = cmd;
  cmdAmount++;
}

/* Checks & runs a command with the split arguments */
char *executeCommand(Game *game, char **args, int8_t spaces) {
  int i;
  char *res = "Not Found";

  for (i = 0; i < cmdAmount + 1; i++) {
    if (strcmp(Commands[i].name, args[0]) || Commands[i].argCount >= spaces)
      continue;

    res = Commands[i].func(game, args);
  }
  free(args);

  return res;
}

/* Split stdin into a command + arguments */
void splitArguments(Game *game, char *str) {

  char **args = NULL;
  char *ptr = strtok(str, " ");
  int spaces = 0;

  while (ptr) {
    if ((args = realloc(args, sizeof(char *) * ++spaces)) == NULL)
      return;

    args[spaces - 1] = ptr;
    ptr = strtok(NULL, " ");
  }

  if ((args = realloc(args, sizeof(char *) * (spaces + 1))) == NULL)
    return;
  args[spaces] = 0;

  printf("%s\n", executeCommand(game, args, spaces));
}
