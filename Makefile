# makefile
#
# Licensed under GPL version 3 or later.
# See LICENSE for copyright information.

include config.mk

all: $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

clean:
	@rm -vf $(OBJ) $(TARGET)
