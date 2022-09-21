/* game.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "game.h"
#include "mem.h"
#include "utils.h"

#include <stdio.h>
#include <unistd.h>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

/* Check if the game is running */
char checkGame(pid_t pid) {
  char fName[FILENAME_MAX];

  sprintf(fName, "/proc/%d/maps", pid);
  return access(fName, R_OK);
}

/* Initalizes the cheat */
void openGame(Game *game, char *name) {
  if ((game->pid = findPid(name)) == -1)
    die("Game not found");

  moduleAddr(game->pid, "/bin/client.so", &game->ClientModule,
             &game->ClientModuleEnd, 1);
  moduleAddr(game->pid, "/bin/engine.so", &game->EngineModule,
             &game->EngineModuleEnd, 1);

  if ((game->ClientModule + game->ClientModuleEnd) == 0)
    die("Could not get the client.so module\n");
  else if ((game->EngineModule + game->ClientModuleEnd) == 0)
    die("Could not get the engine.so module\n");

  printf("Version (%s) of the cheat has loaded\n"
         "%s -> PID: %d\n"
         "client.so -> (%x-%x)\n"
         "engine.so -> (%x-%x)\n",
         VERSION, name, game->pid, game->ClientModule, game->ClientModuleEnd,
         game->EngineModule, game->EngineModuleEnd);

  /* @TODO: replace with sigs */
  game->Offsets = (Offsets){
      0x00ED93E8, /* LPlr Addr | (c2ce13e8 | xx?x) */
      0x7ED829,   /* Glow Addr */
      0x7F3635,   /* Boom Addr */
  };
}

/* Checks if the LocalPlayer exists */
char playerFound(Game *game) {
  if (readAddr(game->pid, game->ClientModule + game->Offsets.PlayerAddr,
               &game->Player.Address, sizeof(game->Player.Address)) == -1)
    return -1;

  return readAddr(game->pid, game->Player.Address, &game->Player,
                  sizeof(game->Player) - sizeof(game->Player.Address));
}