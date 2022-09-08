/* mem.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#define _GNU_SOURCE

#include "../include/mem.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/wait.h>

void moduleAddr(pid_t pid, char *lib, int32_t *start, int32_t *end) {
  char fileName[FILENAME_MAX];
  char buf[248], libName[64], rwxp[4];
  FILE *f;

  sprintf(fileName, "/proc/%d/maps", pid);
  if (lib)
    sprintf(libName, "%s", lib);

  if ((f = fopen(fileName, "r")) == NULL)
    return;

  while (fgets(buf, sizeof(buf), f)) {
    if (lib && !strstr(buf, libName))
      continue;

    sscanf(buf, "%lx-%lx %s", (unsigned long *)start, (unsigned long *)end,
           rwxp);
    if (rwxp[0] == 'r' && rwxp[2] == 'x')
      break;
  }
  fclose(f);

  return;
}

/* Unsafe (Detectable) write func, write instructions */
char pokeAddr(pid_t pid, int32_t addr, char *buf, size_t size) {
  int i = 0, j = size / sizeof(long);
  pokeData data;

  ptrace(PTRACE_ATTACH, pid, 0, 0);
  waitpid(pid, 0, 0);

  for (i = 0; i < j; i++, buf += sizeof(long)) {
    memcpy(data.chars, buf, sizeof(long));
    ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);
  }

  if ((j = size % sizeof(long)) == 0)
    return ptrace(PTRACE_DETACH, pid, 0, 0);

  memcpy(data.chars, buf, j);
  ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);

  return ptrace(PTRACE_DETACH, pid, 0, 0);
}

/* Safe read func */
/* @TODO: replace lseek64 */
char readAddr(pid_t pid, unsigned int addr, void *buf, size_t size) {
  int f;
  int8_t ret;
  char fileName[FILENAME_MAX];

  sprintf(fileName, "/proc/%d/mem", pid);

  if ((f = open(fileName, O_RDONLY)) == -1)
    return -1;

  if ((ret = lseek64(f, addr, SEEK_SET)) != -1)
    read(f, buf, size);

  close(f);
  return ret;
}

/* Scan for a signature through a memory region to get an address */
/* @TODO: if issues arise, add wildcard */
off_t ScanAddr(int32_t start, int32_t end, const char *sig) {
  pokeData data;

  while (start < end) {
    data.val = start;
    if (data.chars[0] == sig[0] && memcmp(data.chars, sig, strlen(sig)) == 1)
      break;

    start++;
  }

  if ((end - 1) == start)
    return 0;

  return start;
}

/* Safe write func, write values */
char writeAddr(pid_t pid, int32_t addr, void *buf, size_t size) {
  int f, ret;
  char fileName[FILENAME_MAX];

  sprintf(fileName, "/proc/%d/mem", pid);

  if ((f = open(fileName, O_WRONLY)) == -1)
    return -1;

  if ((ret = lseek(f, addr, SEEK_SET)) != -1)
    write(f, buf, size);

  close(f);
  return ret;
}