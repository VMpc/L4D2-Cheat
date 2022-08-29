/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/commands.h"
#include "../include/game.h"
#include "../include/utils.h"

#include <string.h>

char *toggleEsp(Game *game, char **args) {
  char *NOPs = "\x90\x90\x90\x90\x90\x90\xFF";
  char *Revert = "\x0F\x83\xD1\x01\x00\x00\x0FF\x90";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + Offsets.GlowAddr, NOPs,
             strlen(NOPs));
    return "Turned on esp";
  } else if (strstr(args[1], "off")) {
    pokeAddr(game->pid, game->clientModule + Offsets.GlowAddr, Revert,
             strlen(Revert));
    return "Turned off esp";
  }

  return "What were you trying to do?";
}

char *toggleNoBoom(Game *game, char **args) {
  char *NOPs = "\x90\x90\x90\x90\x90\x89\x1C\x24";
  char *Revert = "\xE8\x46\x2B\xFF\xFF\x89\x1C\x24";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + Offsets.BoomAddr, NOPs,
             strlen(NOPs));
    return "Turned on no-boom";
  } else if (strstr(args[1], "off")) {
    pokeAddr(game->pid, game->clientModule + Offsets.BoomAddr, Revert,
             strlen(Revert));
    return "Turned off no-boom";
  }

  return "What were you trying to do?";
}
