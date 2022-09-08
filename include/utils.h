/* utils.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>
#include <sys/types.h>

#define die(str)                                                               \
  do {                                                                         \
    puts(str);                                                                 \
    exit(1);                                                                   \
  } while (0)

/* Functions */

char checkGame(pid_t);

void doSleep(int);

pid_t findPid(char *);

char *getLine(char *);

#endif /* _UTILS_H */
