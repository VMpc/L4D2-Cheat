/* game.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "game.h"
#include "keyboard.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Initalizes the cheat */
void openGame(Game *game, char *name)
{
  printf("The cheat has loaded, waiting for L4D2...\n");
  openKeyboard();
  openUinputKeyboard();

  while ((game->pid = findPid(name)) == 0)
    sleep(1);
  printf("Found L4D2 (PID: %d)\n", game->pid);

  while ((game->clientModule = moduleAddr(game->pid, "client.so")) == 0)
    sleep(1);

  printf("Found L4D2s module address (client.so -> %x)\n",
	game->clientModule);
}

/* Checks if our player exists */
char playerFound(Game *game)
{
  return readAddr(game->pid, game->clientModule + Offsets.PlayerAddr,
	&game->Player, sizeof(game->Player));
}

/*
 	* Toggle a cheats flag
	* TODO: Allow ToggleFlag to support any type (Including write to addresses)
*/
void toggleFlag(char *addr, char toggle, char *printStr)
{
  *addr = toggle;
  printf("%s\n", printStr);
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
