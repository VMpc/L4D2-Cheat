/* main.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#define _GNU_SOURCE

#include "commands.h"
#include "handler.h"
#include "keyboard.h"
#include "mem.h"
#include "utils.h"

#include <linux/input.h>

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static Game game;
static Toggle Toggles[] = {
    {KEY_UP, 1, &toggleBhop},
    {KEY_DOWN, 0, &toggleBhop},
    {0, 0, NULL},
};

void handleInput(int);
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

void handleInput(int key) {
  int i;

  for (i = 0; Toggles[i].func != 0; i++) {
    if (Toggles[i].Key == key) {
      (*Toggles[i].func)(&game, Toggles[i].Value);
      return;
    }
  }

  return;
}

/* Main cheat thread */
static void *mainThread(void *_) {
  (void)_; /* ignoring extra thread arg */
  int key;
  int keyF;
  int UinputF;
  
  if ((keyF = openKeyboard()) == -1)
    die("Could not open the input device");

  if ((UinputF = openUinputKeyboard()) == -1)
    die("Could not open /dev/uinput");

  while (checkGame(game.pid) != -1) {
    if ((key = getInput(keyF)) != -1)
      handleInput(key);

    if (playerFound(&game) == -1)
      continue;

    if (game.Bhop &&
        (game.Player.m_fFlags == 131 || game.Player.m_fFlags == 643))
      sendInput(UinputF, KEY_SPACE);

    usleep(100);
  }

  die("Game not running");
  return NULL;
}
