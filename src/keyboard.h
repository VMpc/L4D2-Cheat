/* keyboard.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define MAX_KEY_SIZE 144

/* Structs */
typedef struct Key
{
    char exists;
    char Value;
} KeyS;
extern struct Key KeyList[MAX_KEY_SIZE];

/* Funcs */
void closeKeyboard(void);

void openKeyboard(void);

void openUinputKeyboard(void);

void manageInput(void);

void sendInput(int);

#endif /* _KEYBOARD_H */
