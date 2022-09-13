# config.mk
#
# Licensed under GPL version 3 or later.
# See LICENSE for copyright information.

CC ?= cc

TARGET  := L4D2Cheat
VERSION := $(shell head -n 1 VERSION)

DIR     ?= /usr/local/sbin

ifeq ($(RELEASE),1)
  CPPFLAGS := -DNDEBUG
endif

ifeq ($(DEBUG),1)
  CFLAGS := -O0 -g
else
  CFLAGS := -O3
endif

LDFLAGS  := -pthread
CPPFLAGS += -DVERSION=\"$(VERSION)\"
CFLAGS   += -std=c99 \
            -m32 \
            -pedantic \
            -Wall \
            -Werror \
            -Wextra \
            -Wno-deprecated-declarations \
            -Wmissing-prototypes \
            -Wold-style-definition \
            -Iinclude \
            $(CPPFLAGS)

SRC := src/commands.c \
       src/game.c \
       src/handler.c \
       src/keyboard.c \
       src/main.c \
       src/mem.c \
       src/utils.c

OBJ := $(SRC:.c=.o)
