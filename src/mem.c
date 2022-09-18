/* mem.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#define _GNU_SOURCE

#include "mem.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/wait.h>

/* Grabs a memory range of a module */
void moduleAddr(pid_t pid, char *lib, u_int32_t *start, u_int32_t *end) {
  char fName[FILENAME_MAX];
  char buf[248], lName[64], rwxp[4];
  FILE *f;

  sprintf(fName, "/proc/%d/maps", pid);
  if (lib)
    sprintf(lName, "%s", lib);

  if ((f = fopen(fName, "r")) == NULL)
    return;

  while (fgets(buf, sizeof(buf), f)) {
    if (lib && !strstr(buf, lName))
      continue;

    sscanf(buf, "%lx-%lx %s", (unsigned long *)start, (unsigned long *)end,
           rwxp);
    if (rwxp[0] == 'r' && rwxp[2] == 'x')
      break;
  }
  fclose(f);

  return;
}

/* Unsafe (Detectable, VAC doesn't detect AFAIK) write func, write instructions */
void ptraceWrite(pid_t pid, u_int32_t addr, const char *buf, size_t size) {
  int i = 0, j = size / sizeof(long);
  pokeData data;

  ptrace(PTRACE_ATTACH, pid, 0, 0);
  waitpid(pid, 0, 0);

  for (i = 0; i < j; i++, buf += sizeof(long)) {
    memcpy(data.chars, buf, sizeof(long));
    ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);
  }

  if ((j = size % sizeof(long)) == 0) {
    ptrace(PTRACE_DETACH, pid, 0, 0);
    return;
  }

  memcpy(data.chars, buf, j);
  ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);

  ptrace(PTRACE_DETACH, pid, 0, 0);
  return;
}

/* Safe read func (/proc/pid/mem), read values */
/* @TODO: replace lseek64 */
char readAddr(pid_t pid, u_int32_t addr, void *buf, size_t size) {
  int f;
  int8_t ret;
  char fName[FILENAME_MAX];

  sprintf(fName, "/proc/%d/mem", pid);

  if ((f = open(fName, O_RDONLY)) == -1)
    return -1;

  if ((ret = lseek64(f, addr, SEEK_SET)) != -1)
    ret = read(f, buf, size);

  close(f);
  return ret;
}

/* Scan for a signature through a memory region to get an address */
u_int32_t ScanAddr(u_int32_t start, u_int32_t end, char *sig, char *mask,
                   size_t size) {
  size_t i;
  pokeData data;

  while (start < end) {
    data.val = start;
    for (i = 0; i < size; i++) {
      if (mask[i] != '?' && sig[i] != data.chars[i])
        break;
    }

    if (i == size)
      return start;

    start++;
  }

  return -1;
}

/* Safe write func, write values */
/* @TODO: replace lseek64 */
char writeAddr(pid_t pid, u_int32_t addr, void *buf, size_t size) {
  int f, ret;
  char fName[FILENAME_MAX];

  sprintf(fName, "/proc/%d/mem", pid);

  if ((f = open(fName, O_WRONLY)) == -1)
    return -1;

  if ((ret = lseek64(f, addr, SEEK_SET)) != -1)
    ret = write(f, buf, size);

  close(f);
  return ret;
}
