/* utils.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <sys/types.h>

typedef union PokeUnion {
  long val;
  char chars[sizeof(long)];
} pokeData;

/* Functions */
char checkAllowed(void);

char checkGame(pid_t);

void doSleep(int);

void die(const char *);

pid_t findPid(char *);

char *getLine(char *);

void moduleAddr(pid_t pid, char *lib, int32_t *start, int32_t *end);

char pokeAddr(pid_t, long, char *, int);

char readAddr(pid_t, unsigned int, void *, size_t);

off_t ScanAddr(int32_t, int32_t, const char *);

char writeAddr(pid_t, ssize_t, void *, ssize_t);

#endif /* _UTILS_H */
