/* keyboard.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "game.h"

/* Read KEYPRESS and KEYRELEASE events from /dev/input/eventX */
int getInput(int);
/* Finds a /dev/input device */
int openKeyboard(void);
/* Creates a virtual device */
int openUinputKeyboard(void);
/* Send then release a key */
void sendInput(int, int);

#endif /* _KEYBOARD_H */
