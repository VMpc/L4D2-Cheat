/* keyboard.c
 *
 * Licensed under GPL version 3 or later.
 * See LICENSE for copyright information.
 */

#include "keyboard.h"
#include "utils.h"

#include <fcntl.h>

#include <linux/uinput.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Handles KEYPRESS and KEYRELEASE events */
int getInput(int keyFile) {
  struct input_event ie;
  
  if (read(keyFile, &ie, sizeof(ie)) == -1)
    return -1;

  if (ie.type != EV_KEY || ie.value != 1)
    return -1;

  return ie.code;
}

/* Finds a /dev/input device */
int openKeyboard(void) {
  FILE *f;
  char buf[1024], evName[32];
  char fName[FILENAME_MAX] = "/dev/input/";
  char *ptr;

  if ((f = fopen("/proc/bus/input/devices", "r")) == NULL)
    die("Could not open /proc/bus/input/devices");

  while (fgets(buf, sizeof(buf), f)) {
    if ((ptr = strstr(buf, "Handlers="))) {
      ptr += 9;
      if ((ptr = strstr(buf, "event")))
        ptr = strtok(ptr, " ");

      sprintf(evName, "%s", ptr);
    }

    if (strstr(buf, "EV=120013")) {
      free(ptr);
      break;
    }
  }

  fclose(f);
  strcat(fName, evName);

  return open(fName, O_RDONLY | O_NONBLOCK);
}

/* Creates a virtual device */
/* @TODO Support Alpine Linux */
int openUinputKeyboard(void) {
  struct uinput_setup usetup = {0};
  int uinputF = -1;

  if ((uinputF = open("/dev/uinput", O_WRONLY | O_NONBLOCK)) == -1)
    return uinputF;

  ioctl(uinputF, UI_SET_EVBIT, EV_KEY);
  ioctl(uinputF, UI_SET_KEYBIT, KEY_SPACE);

  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x046d;
  usetup.id.product = 0xc53d;

  strcpy(usetup.name, "Logitech USB Receiver");

  ioctl(uinputF, UI_DEV_SETUP, &usetup);
  ioctl(uinputF, UI_DEV_CREATE);

  return uinputF;
}

/* Send a keyboard event with a specified type */
static int sendEvent(int keyFile, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;

  return write(keyFile, &ie, sizeof(ie));
}

/* Send then release a key */
void sendInput(int keyFile, int keyCode) {
  sendEvent(keyFile, EV_KEY, keyCode, 1);
  sendEvent(keyFile, EV_SYN, SYN_REPORT, 0);
  sendEvent(keyFile, EV_KEY, keyCode, 0);
  sendEvent(keyFile, EV_SYN, SYN_REPORT, 0);
}
