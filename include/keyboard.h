/* keyboard.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/* Structs */
typedef struct {
  char exists;
  char Value;
} Key;

/* Funcs */
char checkKey(int key);

void openKeyboard(void);

void openUinputKeyboard(void);

void manageInput(void);

void sendInput(int);

#endif /* _KEYBOARD_H */
