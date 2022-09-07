/* game.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/game.h"
#include "../include/keyboard.h"
#include "../include/utils.h"

#include <stdio.h>

/* Initalizes the cheat */
void openGame(Game *game, char *name) {

  if ((game->pid = findPid(name)) == -1)
    die("L4D2 not found");
  printf("Found L4D2 (PID: %d)\n", game->pid);

  moduleAddr(game->pid, "/bin/client.so", &game->clientModule,
             &game->clientModuleEnd);
  if ((game->clientModule + game->clientModuleEnd) == 0)
    die("Could not get the client.so module\n");

  openKeyboard();
  openUinputKeyboard();
  printf("Found L4D2s module address (client.so -> (%x-%x))\n",
         game->clientModule, game->clientModuleEnd);
}

/* Checks if our player exists */
char playerFound(Game *game) {
  return readAddr(game->pid, game->clientModule + Offsets.PlayerAddr,
                  &game->Player, sizeof(game->Player));
}

/* Stores all of our addresses/Offsets */
struct OffsetStruct Offsets = {
    /* Addrs */
    0x00ED93E8, /* Player Addr */
    0x7ED829,   /* Glow Addr */
    0x7F3635,   /* Boom Addr */

    /* Offsets */
    0xE0, /* Crouch flag (0XF0 on windows) */
};
