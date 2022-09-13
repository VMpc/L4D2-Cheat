/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "commands.h"
#include "mem.h"

#include <stdio.h>
#include <string.h>

void toggleBhop(Game *game, char bool) {
  game->Bhop = bool;
}

void toggleEsp(Game *game, char **restrict args) {
  const char *NOP = "\x90\x90\x90\x90\x90\x90\xFF";
  const char *REV = "\x0F\x83\xD1\x01\x00\x00\xFF";
  const char *ret;

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, NOP, 7);
    ret = "Turned on esp";
  } else {
    pokeAddr(game->pid, game->clientModule + game->Offsets.GlowAddr, REV, 7);
    ret = "Turned off esp";
  }

  puts(ret);
}

void toggleNoBoom(Game *game, char **restrict args) {
  const char *NOP = "\x90\x90\x90\x90\x90\x89\x1C\x24";
  const char *REV = "\xE8\x46\x2B\xFF\xFF\x89\x1C\x24";
  const char *ret;

  if (strstr(args[1], "on")) {
    pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, NOP, 8);
    ret = "Turned on no-boom";
  } else {
    pokeAddr(game->pid, game->clientModule + game->Offsets.BoomAddr, REV, 8);
    ret = "Turned off no-boom";
  }

  puts(ret);
}
