/* mem.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _MEM_H
#define _MEM_H

#include <sys/types.h>

/* Structs */
typedef union {
  long val;
  char chars[sizeof(long)];
} pokeData;

/* Functions */
void moduleAddr(pid_t pid, char *lib, u_int32_t *, u_int32_t *);

char pokeAddr(pid_t, u_int32_t, char *, size_t);

char readAddr(pid_t, u_int32_t, void *, size_t);

u_int32_t ScanAddr(u_int32_t, u_int32_t, char *, char *, size_t);

char writeAddr(pid_t, u_int32_t, void *, size_t);

#endif /* _MEM_H */
