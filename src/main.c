/* main.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/commands.h"
#include "../include/game.h"
#include "../include/handler.h"
#include "../include/keyboard.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/uinput.h>

#ifndef VERSION
#  define VERSION "No version provided at compile time"
#endif

static void initCommands(void);
static void *mainThread(void *);
static void sigTrap(int);

static Game game;

int main(int argc, char **argv)
{
  pthread_t threadID;

  if (argc == 2 && strcmp(argv[1], "-v") == 0)
  {
    printf("L4D2 Cheat %s\n", VERSION);
    return 0;
  }
  else if (argc != 1)
  {
    fprintf(stderr, "%s: [-v]\n", argv[0]);
    return 1;
  }

  signal(SIGINT, sigTrap);

  checkAllowed();

  openGame(&game, "hl2_linux");

  pthread_create(&threadID, NULL, mainThread, NULL);

  initCommands();

  return 0;
}

/* Create our commands and handler */
static void initCommands(void)
{
  char *line = NULL;

  addCommand("glow", 1, toggleEsp);
  addCommand("noboom", 1, toggleNoBoom);

  while (1)
  {
    printf("> ");
    line = getLine(line);
    splitArguments(&game, line);

    free(line);
  }
}

/* Main cheat thread */
static void *mainThread(void *_)
{
  int playerFlag;
  (void)_; /* ignoring extra thread arg */

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

    readAddr(game.pid,
             game.Player + Offsets.PlrCrouch,
             &playerFlag, sizeof(playerFlag));

    if (playerFlag == 131 || playerFlag == 643)
      sendInput(KEY_SPACE);

    doSleep(10000);
  }
  return NULL;
}

/* Wait for Ctrl + C */
static void sigTrap(int sig)
{
  signal(sig, SIG_IGN);
  closeKeyboard();
}
