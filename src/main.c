/* main.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "../include/handler.h"
#include "../include/keyboard.h"
#include "../include/mem.h"
#include "../include/utils.h"

#include <linux/input.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void *mainThread(void *);
static Game game;

int main(void) {
  pthread_t threadID;
  char *line = NULL;

  if (getuid() != 0)
    die("You must run this program as root");

  openGame(&game, "hl2_linux");

  pthread_create(&threadID, NULL, mainThread, NULL);

  while (1) {
    printf("> ");
    line = getLine(line);
    executeCommand(&game, line);
  }
}

/* Main cheat thread */
static void *mainThread(void *_) {
  (void)_; /* ignoring extra thread arg */

  while (checkGame(game.pid) != -1) {
    manageInput();

    if (playerFound(&game) == -1)
      continue;

    if (checkKey(KEY_UP) == 1 && !game.Bhop)
      game.Bhop = 1;

    else if (checkKey(KEY_DOWN) == 1 && game.Bhop)
      game.Bhop = 0;

    if (!game.Bhop)
      continue;

    if (game.Player.FFlags == 131 || game.Player.FFlags == 643)
      sendInput(KEY_SPACE);

    doSleep(100);
  }

  die("Game not running");
}