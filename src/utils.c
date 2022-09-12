/* utils.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#define _GNU_SOURCE

#include "utils.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Sleep in microseconds */
void doSleep(int ms) {
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = ms;

  select(0, NULL, NULL, NULL, &tv);
}

/* find the PID of a running process */
pid_t findPid(char *name) {
  FILE *f;
  DIR *dir;
  pid_t pid;
  struct dirent *de;
  char fileName[FILENAME_MAX], fileContent[150];

  if ((dir = opendir("/proc")) == NULL)
    die("Could not open /proc directory");

  while ((de = readdir(dir)) != NULL) {
    if ((pid = atoi(de->d_name)) == 0)
      continue;

    sprintf(fileName, "/proc/%d/cmdline", pid);
    if ((f = fopen(fileName, "r")) == NULL)
      continue;

    if (fgets(fileContent, sizeof(fileContent), f) && strstr(fileContent, name))
      break;

    fclose(f);
    pid = -1;
  }

  closedir(dir);
  return pid;
}