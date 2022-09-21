/* mem.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _MEM_H
#define _MEM_H

#include <sys/types.h>

typedef union {
  long val;                 /* Instructions as a number  */
  char chars[sizeof(long)]; /* Instructions in a char array */
} pokeData;

/* Scan func (/proc/pid/maps) */
void moduleAddr(pid_t, char *, u_int32_t *, u_int32_t *, char);
/* Write func (ptrace), instructions */
void ptraceWrite(pid_t, u_int32_t, const char *, size_t);
/* Read func (/proc/pid/mem), values */
char readAddr(pid_t, u_int32_t, void *, size_t);
/* Scan func (memory range), address */
u_int32_t ScanAddr(u_int32_t, u_int32_t, char *, char *, size_t);
/* Write func (/proc/pid/mem), values */
char writeAddr(pid_t, u_int32_t, void *, size_t);

#endif /* _MEM_H */
