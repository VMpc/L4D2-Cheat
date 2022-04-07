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

#include "game.h"
#include "keyboard.h"
#include "utils.h"

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
