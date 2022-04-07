#ifndef _GAME_H
#define _GAME_H
#include "includes.h"


typedef struct
{
  int32_t clientModule; /* client.so */
  pid_t pid;            /* L4D2 pid */
  int32_t Player;       /* ASLR bypassed player addr */

  char doBhop; /* this should be in it's own "options" struct */
} Game;

struct OffsetStruct
{
  int32_t PlayerAddr;
  int32_t GlowAddr;
  int32_t BoomAddr;

  int PlrCrouch;
};
extern struct OffsetStruct Offsets;

void openGame(Game *, char *);

char playerFound(Game *);

void toggleFlag(char *, char, char *);

#endif
