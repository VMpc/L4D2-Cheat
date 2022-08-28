# config.mk
#
# Licensed under GPL version 3 or later.
# See LICENSE for copyright information.

CC = cc

TARGET  = L4D2Cheat
VERSION = 0.0.1

LDFLAGS = -pthread
CFLAGS  = -ansi \
          -m32 \
          -pedantic \
          -Wall \
          -Wextra \
          -Wno-deprecated-declarations \
          -Wmissing-prototypes \
          -Wold-style-definition \
          -DVERSION=$(VERSION)

SRC = src/commands.c src/game.c src/handler.c src/keyboard.c src/main.c src/utils.c
OBJ = $(SRC:.c=.o)
