/* commands.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "game.h"

/* Handle the bhop hotkey */
void toggleBhop(Game *restrict, char);
/* Handle the bhopdelay command */
void bhopDelay(Game *restrict, char **restrict);
/* Handle the glow command */
void toggleEsp(Game *, char **restrict);
/* Handle the noboom command */
void toggleNoBoom(Game *, char **restrict);

#endif /* _COMMANDS_H */
