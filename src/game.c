/* game.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "game.h"
#include "keyboard.h"
#include "mem.h"
#include "utils.h"

#include <stdio.h>
#include <unistd.h>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

/* Check if the game is running */
char checkGame(pid_t pid) {
  char fileName[FILENAME_MAX];

  sprintf(fileName, "/proc/%d/maps", pid);
  return access(fileName, R_OK);
}

/* Initalizes the cheat */
void openGame(Game *game, char *name) {
  if ((game->pid = findPid(name)) == -1)
    die("Game not found");

  moduleAddr(game->pid, "/bin/client.so", &game->clientModule,
             &game->clientModuleEnd);
  if ((game->clientModule + game->clientModuleEnd) == 0)
    die("Could not get the client.so module\n");

  openKeyboard();
  openUinputKeyboard();

  printf("Version (%s) of the cheat has loaded\n%s -> PID: %d\nclient.so -> "
         "(%x-%x)\n",
         VERSION, name, game->pid, game->clientModule, game->clientModuleEnd);

  /* @TODO: replace with sigs */
  game->Offsets = (Offsets){
      0x00ED93E8, /* LPlr Addr | (c2ce13e8 | xx?x) */
      0x7ED829,   /* Glow Addr */
      0x7F3635,   /* Boom Addr */
  };
}

/* Checks if our player exists */
char playerFound(Game *game) {
  if (readAddr(game->pid, game->clientModule + game->Offsets.PlayerAddr,
               &game->Player.Address, sizeof(game->Player.Address)) == -1)
    return -1;

  return readAddr(game->pid, game->Player.Address, &game->Player,
                  sizeof(game->Player) - sizeof(game->Player.Address));
}