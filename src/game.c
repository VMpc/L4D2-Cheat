/* game.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/game.h"
#include "../include/keyboard.h"
#include "../include/utils.h"

#include <stdio.h>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

/* Initalizes the cheat */
void openGame(Game *game, char *name) {
  die((game->pid = findPid(name)) == -1, "L4D2 not found");

  moduleAddr(game->pid, "/bin/client.so", &game->clientModule,
             &game->clientModuleEnd);
  die((game->clientModule + game->clientModuleEnd) == 0,
      "Could not get the client.so module\n");

  openKeyboard();
  openUinputKeyboard();

  printf(
      "Version (%s) of the cheat has loaded (PID: %d, client.so -> (%x-%x))\n",
      VERSION, game->pid, game->clientModule, game->clientModuleEnd);
}

/* Checks if our player exists */
char playerFound(Game *game) {
  if (readAddr(game->pid, game->clientModule + Offsets.PlayerAddr,
               &game->Player.Address, sizeof(game->Player.Address)) == -1)
    return -1;

  return readAddr(game->pid, game->Player.Address, &game->Player,
                  sizeof(game->Player) - sizeof(game->Player.Address));
}

/* Stores all of our addresses/Offsets */
struct OffsetStruct Offsets = {
    0x00ED93E8, /* Player Addr */
    0x7ED829,   /* Glow Addr */
    0x7F3635,   /* Boom Addr */
};
