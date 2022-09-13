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

#include <linux/input.h>


static Command commands[] = {
    {"glow", &toggleEsp},
    {"noboom", &toggleNoBoom},
};

static Toggle Toggles[] = {
    {KEY_UP, 1, &toggleBhop},
    {KEY_DOWN, 0, &toggleBhop},
    {0, 0, NULL},
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

void handleInput(Game *restrict game, int key) {
  int i;

  for (i = 0; Toggles[i].func != 0; i++) {
    if (Toggles[i].Key == key) {
      (*Toggles[i].func)(game, Toggles[i].Value);
      return;
    }
  }

  return;
}

/* Split stdin into a command + arguments */
char **splitArguments(char str[]) {
  char **ret = malloc(8);
  char *ptr = strtok(str, " ");
  int i;

  for (i = 0; ptr != NULL; i++) {
    ret[i] = ptr;
    ptr = strtok(NULL, " ");
  }

  free(ptr);
  return ret;
}