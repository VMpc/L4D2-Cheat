/* utils.h
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <sys/types.h>

/* print then exit the program */
void die(char *);
/* find the PID of a running process */
pid_t findPid(char *);

#endif /* _UTILS_H */
