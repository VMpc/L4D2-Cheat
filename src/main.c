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

#include "commands.h"
#include "game.h"
#include "handler.h"
#include "keyboard.h"
#include "main.h"
#include "utils.h"

Game game;


/* Create our commands and handler */
void initCommands(void)
{
  addCommand("glow", 1, toggleEsp);
  addCommand("noboom", 1, toggleNoBoom);

  while (1)
  {
	char *line = NULL;

    printf("> ");
    line = getLine(line);
    splitArguments(&game, line);

    free(line);
  }
}

/* Main cheat thread */
void *mainThread(void *_)
{
  int playerFlag;
  (void)_;

  while (1)
  {
    manageInput();

    if (playerFound(&game) == 0)
      continue;

    if (KeyList[KEY_UP].Value == 1 && !game.doBhop)
      toggleFlag(&game.doBhop, 1, "Bhop enabled");

    if (KeyList[KEY_DOWN].Value == 1 && game.doBhop)
      toggleFlag(&game.doBhop, 0, "Bhop disabled");

    if (!game.doBhop)
		continue;

    readAddr(game.pid, game.Player + Offsets.PlrCrouch,
		&playerFlag, sizeof(playerFlag));

    if (playerFlag == 131 || playerFlag == 643)
      sendInput(KEY_SPACE);

    doSleep(10000);
  }
}

/* Wait for Ctrl + C */
void sigTrap(int sig)
{
  signal(sig, SIG_IGN);
  closeKeyboard();
}

int main(void)
{
  pthread_t threadID;

  signal(SIGINT, sigTrap);

  checkAllowed();

  openGame(&game, "hl2_linux");

  pthread_create(&threadID, NULL, mainThread, (void *)(NULL));

  initCommands();

  return 0;
}
