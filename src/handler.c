/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include "includes.h"
#include "game.h"
#include "handler.h"

int cmdAmount = -1;
struct Cmd Commands[MAX_COMMANDS];

/* Initializes a command for the handler */
void addCommand(char *cmdName, int argCount, char *(*func)(Game *, char **))
{
  Command cmd;
  cmd.name = cmdName;
  cmd.argCount = argCount;
  cmd.func = func;
  Commands[cmdAmount + 1] = cmd;
  cmdAmount++;
}

/* Checks & runs a command with the split arguments */
char *executeCommand(Game *game, char **args, int spaces)
{
  int i;
  char *res = "Not Found";

  for (i = 0; i < cmdAmount + 1; i++)
  {
    if (strcmp(Commands[i].name, args[0]) || Commands[i].argCount >= spaces)
      continue;

    res = Commands[i].func(game, args);
  }
  free(args);

  return res;
}

/* Split stdin into a command + arguments */
void splitArguments(Game *game, char *str)
{

  char **args = NULL, *ptr = strtok(str, " ");
  int spaces = 0;

  while (ptr)
  {
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
