/* utils.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */
#define _GNU_SOURCE

#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

void die(char doExit, const char *str) {
  if (!doExit)
    return;

  printf("%s\n", str);
  exit(1);
}

/* Check if the running user is root */
char checkAllowed(void) { return getuid() == 1000; }

/* Check if the game is running */
char checkGame(pid_t pid) {
  char fileName[FILENAME_MAX];
  DIR *dir;
  sprintf(fileName, "/proc/%d", pid);

  if ((dir = opendir(fileName)) != NULL) {
    closedir(dir);
    return 0;
  }

  return -1;
}

/* Sleep in microseconds */
void doSleep(int ms) {
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = ms;

  select(0, NULL, NULL, NULL, &tv);
}

/* find the PID of a running process */
pid_t findPid(char *name) {
  struct dirent *de;
  DIR *dir;
  FILE *f;
  char fileName[FILENAME_MAX], fileContent[150];
  pid_t pid;

  die((dir = opendir("/proc")) == NULL, "Could not open /proc directory");

  while ((de = readdir(dir)) != NULL) {
    if ((pid = atoi(de->d_name)) == 0)
      continue;

    sprintf(fileName, "/proc/%d/cmdline", pid);
    if ((f = fopen(fileName, "r")) == NULL)
      continue;

    fgets(fileContent, sizeof(fileContent), f);
    fclose(f);

    if (strstr(fileContent, name))
      break;

    pid = -1;
  }

  closedir(dir);
  return pid;
}

/* Reads data from stdin until a new line */
char *getLine(char *line) {
  int c;
  size_t maxSize = 0;
  size_t length = 0;
  char *tempStr = NULL;

  while (((c = getchar()) != '\n') && (c != EOF)) {
    if ((length + 1) >= maxSize) {
      maxSize = (maxSize == 0) ? 2 : maxSize * 2;

      if ((tempStr = realloc(line, maxSize * sizeof(char))) == NULL)
        return line;

      line = tempStr;
    }

    line[length] = (char)c;
    length++;
  }

  line[length + 1] = '\0';

  if ((tempStr = realloc(line, (length + 1) * sizeof(char))) == NULL)
    return line;

  line = tempStr;

  return line;
}

/* Gets the start - end of a module */
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
char pokeAddr(pid_t pid, long addr, char *buf, int size) {
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
  int f, ret;
  char fileName[FILENAME_MAX];

  sprintf(fileName, "/proc/%d/mem", pid);

  if ((f = open(fileName, O_RDONLY)) == -1)
    ret = -1;

  if (lseek64(f, addr, SEEK_SET) == -1)
    ret = -1;

  if (read(f, buf, size) == -1)
    ret = -1;

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
char writeAddr(pid_t pid, ssize_t addr, void *buf, ssize_t size) {
  int f, ret;
  char fileName[FILENAME_MAX];

  sprintf(fileName, "/proc/%d/mem", pid);

  if ((f = open(fileName, O_WRONLY)) == -1)
    ret = -1;

  if (lseek(f, addr, SEEK_SET) == -1)
    ret = -1;

  if (write(f, buf, size) == -1)
    ret = -1;

  close(f);

  return ret;
}
