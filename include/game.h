/* game.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _GAME_H
#define _GAME_H

#include <sys/types.h>

typedef struct {
  u_int32_t PlayerAddr;
  u_int32_t GlowAddr;
  u_int32_t BoomAddr;
} Offsets;

typedef struct {
  char _1[0xDC]; /* 0xDC */
  int Health;    /* Health (0xEC on windows) */

  char _2[0x0]; /* 0xE0 */
  int FFlags;   /* Crouch flag (0xF0 on windows) */

  u_int32_t Address; /* The ASLR bypassed address of the LocalPlayer */
} Player;

typedef struct {
  pid_t pid; /* Game pid */

  u_int32_t clientModule;    /* client.so */
  u_int32_t clientModuleEnd; /* client.so end */

  Offsets Offsets; /* Game offsets */
  Player Player;   /* The player struct */

  char doBhop; /* this should be in it's own "options" struct */
} Game;

char checkGame(pid_t);

void openGame(Game *, char *);

char playerFound(Game *);

#endif /* _GAME_H */
