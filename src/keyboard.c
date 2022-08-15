/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include "includes.h"
#include "keyboard.h"
#include "utils.h"

int fd, uinputfd;
struct Key KeyList[MAX_KEY_SIZE];

/* Frees the virtual keyboard */
void closeKeyboard(void)
{
  ioctl(uinputfd, UI_DEV_DESTROY);
  close(uinputfd);
}

/* Finds a /dev/input device */
void openKeyboard(void) {
  FILE *f;
  char eventName[9], fileName[25], lineBuf[1024];

  if ((f = fopen("/proc/bus/input/devices", "r")) == NULL)
    exitProgram("(openKeyboard) could not open /proc/bus/input/devices", 1);

  while (fgets(lineBuf, sizeof(lineBuf), f)) {
    char *ptr;
    char *ptr2;

    if ((ptr = strstr(lineBuf, "Handlers="))) {
      ptr += strlen("Handlers=");
      if ((ptr = strstr(ptr, "event"))) {
        if ((ptr2 = strchr(ptr, ' ')))
          *ptr2 = '\0';
        sprintf(eventName, "%s", ptr);
      }
    }
    if (strstr(lineBuf, "EV=120013"))
      break;
  }

  fclose(f);

  sprintf(fileName, "/dev/input/%s", eventName);
  if ((fd = open(fileName, O_RDONLY | O_NONBLOCK)) == -1)
    exitProgram("(openKeyboard) could not open the input device", 1);
}

/* Creates a virtual device */
/* @TODO Support Alpine Linux */
void openUinputKeyboard(void)
{
  struct uinput_setup usetup = {0};

  if ((uinputfd = open("/dev/uinput", O_WRONLY | O_NONBLOCK)) == -1)
    exitProgram("/dev/uninput error", 1);

  ioctl(uinputfd, UI_SET_EVBIT, EV_KEY);
  ioctl(uinputfd, UI_SET_KEYBIT, KEY_SPACE);

  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;
  usetup.id.product = 0x5678;

  strcpy(usetup.name, "We are not cheating!");

  ioctl(uinputfd, UI_DEV_SETUP, &usetup);
  ioctl(uinputfd, UI_DEV_CREATE);
}

/* Handles KEYPRESS and KEYRELEASE events */
void manageInput(void)
{
  struct input_event ie;
  int bytes;

  if ((bytes = read(fd, &ie, sizeof(ie))) < 1 && ie.type != EV_KEY)
    return;

  if (KeyList[ie.code].exists == 0)
  {
    KeyS key;
    key.exists = 1;
    key.Value = ie.value;
    KeyList[ie.code] = key;
    return;
  }

  KeyList[ie.code].Value = ie.value;
}


/* Send a keyboard event with a specified type */
static void sendEvent(int type, int code, int val)
{
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;

  write(uinputfd, &ie, sizeof(ie));
}

/* Send then release a key */
void sendInput(int keyCode)
{
  sendEvent(EV_KEY, keyCode, 1);
  sendEvent(EV_SYN, SYN_REPORT, 0);
  sendEvent(EV_KEY, keyCode, 0);
  sendEvent(EV_SYN, SYN_REPORT, 0);
}
