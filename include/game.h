/* game.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _GAME_H
#define _GAME_H

#include <sys/types.h>

typedef struct {
  u_int32_t PlayerAddr; /* The static address of the LocalPlayer */
  u_int32_t GlowAddr;   /* The static address of where to nop for Glow */
  u_int32_t BoomAddr;   /* The static address of where to nop for Noboom */
} Offsets;

/* TODO: dump this entire struct (and structs related to it) */
typedef struct {
  char unk210[210]; /* skip the first 210 elements of the struct */
  char *m_target;
  int m_iMaxHealth; /* 224 */
  int m_iHealth;    /* 228 */
  int m_fFlags;     /* 232 */

  u_int32_t Address; /* The ASLR bypassed address of the LocalPlayer */
} Player;

typedef struct {
  pid_t pid; /* Game pid */

  __uint32_t ClientModule;   /* client.so */
  u_int32_t ClientModuleEnd; /* client.so end */
  u_int32_t EngineModule;    /* engine.so */
  u_int32_t EngineModuleEnd; /* engine.so end */

  Offsets Offsets; /* Game offsets */
  Player Player;   /* The player struct */

  char Bhop; /* this should be in it's own "options" struct */
} Game;

/* Check if the game is running */
char checkGame(pid_t);
/* Initalizes the cheat */
void openGame(Game *, char *);
/* Checks if the LocalPlayer exists */
char playerFound(Game *);

#endif /* _GAME_H */
