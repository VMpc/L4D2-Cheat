/* keyboard.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/handler.h"
#include "../include/commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Command commands[] = {
    {"glow", &toggleEsp},
    {"noboom", &toggleNoBoom},
};

/* Checks & runs a command with the split arguments */
void executeCommand(Game *restrict game, char *restrict str) {
  int i = 0;
  char **args = splitArguments(str);

  while (commands[i].func != 0) {
    if (!strcmp(commands[i].name, args[0]))
      return (*commands[i].func)(game, args);
    i++;
  }

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