/* keyboard.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "game.h"

int getInput(int);

int openKeyboard(void);

int openUinputKeyboard(void);

void sendInput(int, int);

#endif /* _KEYBOARD_H */
