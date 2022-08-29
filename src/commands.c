/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/commands.h"
#include "../include/game.h"
#include "../include/utils.h"

#include <string.h>

#define NOP (char)0x90

char *toggleEsp(Game *game, char **args) {
  char NOPs[7] = {NOP, NOP, NOP, NOP, NOP, NOP, (char)0xFF};

  char Revert[8] = {(char)0x0F, (char)0x83, (char)0xD1, (char)0x01,
                    (char)0x00, (char)0x00, (char)0xFF, NOP};

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + Offsets.GlowAddr, NOPs, 7);
    return "Turned on esp";
  } else if (strstr(args[1], "off")) {
    pokeAddr(game->pid, game->clientModule + Offsets.GlowAddr, Revert, 8);
    return "Turned off esp";
  }

  return "What were you trying to do?";
}

char *toggleNoBoom(Game *game, char **args) {
  char NOPs[8] = {NOP, NOP, NOP, NOP, NOP, (char)0x89, (char)0x1C, (char)0x24};

  char Revert[8] = {(char)0xE8, (char)0x46, (char)0x2B, (char)0xFF,
                    (char)0xFF, (char)0x89, (char)0x1C, (char)0x24};

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + Offsets.BoomAddr, NOPs, 8);
    return "Turned on no-boom";
  } else if (strstr(args[1], "off")) {
    pokeAddr(game->pid, game->clientModule + Offsets.BoomAddr, Revert, 8);
    return "Turned off no-boom";
  }

  return "What were you trying to do?";
}
