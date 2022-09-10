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
Command Commands[MAX_COMMANDS];

/* Initializes a command for the handler */
void addCommand(char *restrict name, int8_t count, cmdfunc func) {
  Command cmd;
  cmd.name = name;
  cmd.argCount = count;
  cmd.func = func;
  Commands[cmdAmount + 1] = cmd;
  cmdAmount++;
}

/* Checks & runs a command with the split arguments */
void executeCommand(Game *game, char **args, int8_t spaces) {
  int i;
  for (i = 0; i < cmdAmount + 1; i++) {
    if (strcmp(Commands[i].name, args[0]) || Commands[i].argCount >= spaces)
      continue;

    if (Commands[i].func == NULL) {
      puts("Command not found.");
      break;
    }

    Commands[i].func(game, args);
  }

  free(args);
}

/* Split stdin into a command + arguments */
void splitArguments(Game *restrict game, char *restrict str) {

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

  executeCommand(game, args, spaces);
}
