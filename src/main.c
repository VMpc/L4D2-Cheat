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

#include <linux/input.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void initCommands(void);
static void *mainThread(void *);
static void sigTrap(int);

static Game game;

int main(void) {
  pthread_t threadID;
  if (getuid() != 0)
    die("You must run this program as root");

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
  }
  free(line);
}

/* Main cheat thread */
static void *mainThread(void *_) {
  (void)_; /* ignoring extra thread arg */

  while (1) {
    if (checkGame(game.pid) == -1)
      die("Game is not running");

    manageInput();

    if (playerFound(&game) == -1)
      continue;

    if (KeyList[KEY_UP].Value == 1 && !game.doBhop)
      game.doBhop = 1;

    else if (KeyList[KEY_DOWN].Value == 1 && game.doBhop)
      game.doBhop = 0;

    if (!game.doBhop)
      continue;

    if (game.Player.FFlags == 131 || game.Player.FFlags == 643)
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
