/* utils.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#define _GNU_SOURCE

#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <pwd.h>
#include <linux/uinput.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

void die(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
    fputc(' ', stderr);
    perror(NULL);
  } else {
    fputc('\n', stderr);
  }

  exit(1);
}

/* Check if the user has the required permissions to run this program */
void checkAllowed(void)
{
  FILE *f;
  char line[50];
  struct passwd *pw;
  uid_t uid;

  if ((uid = getuid()) == 0)
    return;

  if ((f = fopen("/proc/sys/kernel/yama/ptrace_scope", "r")) == NULL)
    die("(checkAllowed) could not open the prace_scope file");

  if (fgetc(f) >= 49 && fgetc(f) <= 51)
    die("You need to run this program as root");
  fclose(f);

  if ((f = fopen("/etc/group", "r")) == NULL)
    die("(checkAllowed) could not open /etc/group");

  if ((pw = getpwuid(uid)) == NULL)
    die("pwuid error: ");

  while (fgets(line, sizeof(line), f))
    if (!(strstr(line, "input") && strstr(line, pw->pw_name)))
      die("You need to run this program as root");

  free(pw);
  fclose(f);
}

/* Sleep in microseconds */
void doSleep(int ms)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = ms;
  select(0, NULL, NULL, NULL, &tv);
}

/* find the procid of a running process */
pid_t findPid(char *name)
{
  struct dirent *de;
  DIR *dir;
  FILE *f;
  char fileName[20], fileContent[150];
  pid_t pid;

  if ((dir = opendir("/proc")) == NULL)
    die("Could not open /proc directory");

  while ((de = readdir(dir)) != NULL)
  {
    if ((pid = atoi(de->d_name)) == 0)
      continue;

    sprintf(fileName, "/proc/%d/cmdline", pid);
    if ((f = fopen(fileName, "r")) == NULL)
      continue;

    fgets(fileContent, sizeof(fileContent), f);
    fclose(f);

    if (strstr(fileContent, name))
      break;

    pid = 0;
  }

  closedir(dir);
  return pid;
}

/* Reads data from stdin until a new line */
char *getLine(char *line)
{
  int c;
  size_t maxSize = 0;
  size_t length = 0;
  char *tempStr = NULL;

  while (((c = getchar()) != '\n') && (c != EOF))
  {
    if ((length + 1) >= maxSize)
    {
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

/* Grabs a the base address of a shared object */
int32_t moduleAddr(pid_t pid, char *library)
{
  char buf[200], libraryName[20], mapFilename[20];
  FILE *f;

  sprintf(mapFilename, "/proc/%d/maps", pid);
  if (library)
    sprintf(libraryName, "/bin/%s", library);

  if ((f = fopen(mapFilename, "r")) == NULL)
    return 0;

  while (fgets(buf, sizeof(buf), f) != NULL)
  {
    if (library && !(strstr(buf, libraryName)))
      continue;

    break;
  }
  fclose(f);

  return strtoul(buf, NULL, 16);
}

/* Unsafe write func, bypasses page protection */
char pokeAddr(pid_t pid, long addr, char *buf, int size)
{
  int i = 0, j = size / sizeof(long);
  pokeData data;

  ptrace(PTRACE_ATTACH, pid, 0, 0);
  waitpid(pid, 0, 0);

  for (i = 0; i < j; i++, buf += sizeof(long))
  {
    memcpy(data.chars, buf, sizeof(long));
    ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);
  }

  j = size % sizeof(long);
  if (j == 0)
    return ptrace(PTRACE_DETACH, pid, 0, 0);

  memcpy(data.chars, buf, j);
  ptrace(PTRACE_POKEDATA, pid, addr + i * 4, data.val);

  return ptrace(PTRACE_DETACH, pid, 0, 0);
}

/* Safe read func */
char readAddr(pid_t pid, ssize_t addr, void *buf, ssize_t size)
{
  struct iovec local[1];
  struct iovec remote[1];

  local[0].iov_base = buf;
  local[0].iov_len = size;
  remote[0].iov_base = (void *)addr;
  remote[0].iov_len = size;

  return process_vm_readv(pid, local, 1, remote, 1, 0) == size;
}

/* Safe write func, does not bypass page protection */
char writeAddr(pid_t pid, ssize_t addr, void *buf, ssize_t size)
{
  struct iovec local[1];
  struct iovec remote[1];

  local[0].iov_base = buf;
  local[0].iov_len = size;
  remote[0].iov_base = (void *)addr;
  remote[0].iov_len = size;

  return process_vm_writev(pid, local, 1, remote, 1, 0) == size;
}
