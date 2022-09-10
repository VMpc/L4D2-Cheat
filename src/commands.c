/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/commands.h"
#include "../include/game.h"
#include "../include/mem.h"

#include <stdio.h>
#include <string.h>

void toggleEsp(Game *game, char **restrict args) {
  char *NOP = "\x90\x90\x90\x90\x90\x90\xFF";
  char *REV = "\x0F\x83\xD1\x01\x00\x00\xFF";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, NOP, 7);
    puts("Turned on esp");
    return;
  }

  pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, REV, 7);
  puts("Turned off esp");
}

void toggleNoBoom(Game *game, char **restrict args) {
  char *NOP = "\x90\x90\x90\x90\x90\x89\x1C\x24";
  char *REV = "\xE8\x46\x2B\xFF\xFF\x89\x1C\x24";

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, NOP, 7);
    puts("Turned on no-boom");
  }

  pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, REV, 7);
  puts("Turned off no-boom");
}
