/* commands.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "commands.h"
#include "mem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Handle the bhop hotkey */
void toggleBhop(Game *restrict game, char bool) { game->Options.Bhop = bool; }

/* Handle the bhopdelay command */
void bhopDelay(Game *restrict game, char **restrict args) {
  if (strstr(args[1], "on")) {
    game->Options.BhopDelay = 1;
    puts("Turned on bhop delay");
  } else {
    game->Options.BhopDelay = 0;
    puts("Turned off bhop delay");
  }
}

/* Handle the glow command */
void toggleEsp(Game *game, char **restrict args) {
  const char *NOP = "\x90\x90\x90\x90\x90\x90\xFF";
  const char *REV = "\x0F\x83\xD1\x01\x00\x00\xFF";

  if (strstr(args[1], "on")) {
    ptraceWrite(game->pid, game->ClientModule + game->Offsets.GlowAddr, NOP, 7);
    puts("Turned on esp");
  } else {
    ptraceWrite(game->pid, game->ClientModule + game->Offsets.GlowAddr, REV, 7);
    puts("Turned off esp");
  }
}

/* Handle the noboom command */
void toggleNoBoom(Game *game, char **restrict args) {
  const char *NOP = "\x90\x90\x90\x90\x90\x89\x1C\x24";
  const char *REV = "\xE8\x46\x2B\xFF\xFF\x89\x1C\x24";

  if (strstr(args[1], "on")) {
    ptraceWrite(game->pid, game->ClientModule + game->Offsets.BoomAddr, NOP, 8);
    puts("Turned on no-boom");
  } else {
    ptraceWrite(game->pid, game->ClientModule + game->Offsets.BoomAddr, REV, 8);
    puts("Turned off no-boom");
  }
}
