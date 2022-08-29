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

#include <linux/uinput.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

static void initCommands(void);
static void *mainThread(void *);
static void sigTrap(int);

static Game game;

int main(void) {
  pthread_t threadID;

  if (checkAllowed() != 0)
    die("(checkAllowed) You must run this program as root");

  printf("Version (%s) of the cheat has loaded\n", VERSION);
  openGame(&game, "hl2_linux");

  pthread_create(&threadID, NULL, mainThread, NULL);
  initCommands();

  signal(SIGINT, sigTrap);
  return 0;
}

/* Create our commands and handler */
static void initCommands(void) {
  char *line = NULL;

  addCommand("glow", 1, toggleEsp);
  addCommand("noboom", 1, toggleNoBoom);

  while (1) {
    printf("> ");
    line = getLine(line);
    splitArguments(&game, line);

    free(line);
  }
}

/* Main cheat thread */
static void *mainThread(void *_) {
  int playerFlag;
  (void)_; /* ignoring extra thread arg */

  while (1) {
    manageInput();

    if (playerFound(&game) == 0)
      continue;

    if (KeyList[KEY_UP].Value == 1 && !game.doBhop)
      game.doBhop = 1;

    else if (KeyList[KEY_DOWN].Value == 1 && game.doBhop)
      game.doBhop = 0;

    if (!game.doBhop)
      continue;

    readAddr(game.pid, game.Player + Offsets.PlrCrouch, &playerFlag,
             sizeof(playerFlag));

    if (playerFlag == 131 || playerFlag == 643)
      sendInput(KEY_SPACE);

    doSleep(100);
  }

  return 0;
}

/* Wait for Ctrl + C */
static void sigTrap(int sig) {
  signal(sig, SIG_IGN);
  closeKeyboard();
}
