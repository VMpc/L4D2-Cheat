/* utils.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#define _GNU_SOURCE

#include "../include/utils.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  char c;
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
