/* mem.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _MEM_H
#define _MEM_H

#include <sys/types.h>

/* Structs */
typedef union PokeUnion {
  long val;
  char chars[sizeof(long)];
} pokeData;

/* Functions */
void moduleAddr(pid_t pid, char *lib, int32_t *start, int32_t *end);

char pokeAddr(pid_t, int32_t, char *, size_t);

char readAddr(pid_t, u_int32_t, void *, size_t);

off_t ScanAddr(int32_t, int32_t, const char *);

char writeAddr(pid_t, u_int32_t, void *, size_t);

#endif /* _MEM_H */
