/* main.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#define _GNU_SOURCE

#include "handler.h"
#include "keyboard.h"
#include "mem.h"
#include "utils.h"

#include <linux/input.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void *mainThread(void *);
static Game game;

int main(void) {
  pthread_t threadID;
  char str[1024];

  if (getuid() != 0)
    die("You must run this program as root");

  openGame(&game, "hl2_linux");

  pthread_create(&threadID, NULL, mainThread, NULL);

  while (1) {
    fputs("> ", stdout);

    if (fgets(str, 1024, stdin))
      executeCommand(&game, str);
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

    usleep(100);
  }

  die("Game not running");
  return NULL;
}