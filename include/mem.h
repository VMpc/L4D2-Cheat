/* mem.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _MEM_H
#define _MEM_H

#include <sys/types.h>

typedef union {
  long val;
  char chars[sizeof(long)];
} pokeData;

/* Grabs a memory range of a module */
void moduleAddr(pid_t, char *, u_int32_t *, u_int32_t *);

/* Unsafe (Detectable, VAC doesn't detect AFAIK) write func, write instructions */
void ptraceWrite(pid_t, u_int32_t, const char *, size_t);
/* Safe read func (/proc/pid/mem), read values */
char readAddr(pid_t, u_int32_t, void *, size_t);
/* Scan for a signature through a memory region to get an address */
u_int32_t ScanAddr(u_int32_t, u_int32_t, char *, char *, size_t);
/* Safe write func (/proc/pid/mem), write values */
char writeAddr(pid_t, u_int32_t, void *, size_t);

#endif /* _MEM_H */
