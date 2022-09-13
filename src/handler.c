/* keyboard.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "handler.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Command commands[] = {
    {"glow", &toggleEsp},
    {"noboom", &toggleNoBoom},
};

/* Checks & runs a command with the split arguments */
void executeCommand(Game *restrict game, char *restrict str) {
  int i;
  char **args = splitArguments(str);

  for (i = 0; commands[i].func != 0; i++) {
    if (!strcmp(commands[i].name, args[0])) {
      (*commands[i].func)(game, args);
      free(args);
      return;
    }
  }
  
  free(args);
  puts("No command found");
}

/* Split stdin into a command + arguments */
char **splitArguments(char str[]) {
  char **ret = malloc(8);
  char *ptr = strtok(str, " ");
  int i = 0;

  for (i = 0; ptr != NULL; i++) {
    ret[i] = ptr;
    ptr = strtok(NULL, " ");
  }

  free(ptr);
  return ret;
}