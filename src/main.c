/* main.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#include "handler.h"
#include "keyboard.h"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static Game game;
static void *mainThread(void *);

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
  int key, keyF, uinputF;

  /* For the randomized bhop delay */
  srand(time(NULL));

  if ((keyF = openKeyboard()) == -1)
    die("Could not open the input device");

  if ((uinputF = openUinputKeyboard()) == -1)
    die("Could not open /dev/uinput");

  while (checkGame(game.pid) != -1) {
    if ((key = getInput(keyF)) != -1)
      handleInput(&game, key);

    if (playerFound(&game) == -1)
      continue;

    if ((game.Options.Bhop) &&
        (game.Player.m_fFlags == 131 || game.Player.m_fFlags == 643)) {

      /* Add some random noise to try throwing off anticheats */
      if (!game.Options.BhopDelay || rand() % 10 <= 5)
        sendInput(uinputF, 57); /* KEY_SPACE */
    }

    sleep(0);
  }

  die("Game not running");
  return NULL;
}