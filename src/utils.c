/* utils.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "utils.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(char *str) {
  puts(str);
  exit(1);
}

/* find the PID of a running process */
pid_t findPid(char *name) {
  FILE *f;
  DIR *dir;
  pid_t pid;
  struct dirent *de;
  char fName[FILENAME_MAX], buf[256];

  if ((dir = opendir("/proc")) == NULL)
    die("Could not open /proc directory");

  while ((de = readdir(dir)) != NULL) {
    if ((pid = atoi(de->d_name)) == 0)
      continue;

    sprintf(fName, "/proc/%d/cmdline", pid);
    if ((f = fopen(fName, "r")) == NULL)
      continue;

    if (fgets(buf, sizeof(buf), f) && strstr(buf, name))
      break;

    fclose(f);
    pid = -1;
  }

  closedir(dir);
  return pid;
}
