/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/commands.h"
#include "../include/game.h"
#include "../include/mem.h"

#include <string.h>

char *toggleEsp(Game *game, char **args) {
  char *NOPs = "\x90\x90\x90\x90\x90\x90\xFF";
  char *Revert = "\x0F\x83\xD1\x01\x00\x00\xFF";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, NOPs,
             strlen(NOPs));
    return "Turned on esp";
  }

  pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, Revert,
           strlen(Revert));
  return "Turned off esp";
}

char *toggleNoBoom(Game *game, char **args) {
  char *NOPs = "\x90\x90\x90\x90\x90\x89\x1C\x24";
  char *Revert = "\xE8\x46\x2B\xFF\xFF\x89\x1C\x24";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, NOPs,
             strlen(NOPs));
    return "Turned on no-boom";
  }

  pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, Revert,
           strlen(Revert));
  return "Turned off no-boom";
}
