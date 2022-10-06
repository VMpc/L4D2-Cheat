include config.mk

all: $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

install: all
	mkdir -p $(DIR)
	cp -f $(TARGET) $(DIR)/$(TARGET)
	chmod 755 $(DIR)/$(TARGET)

uninstall:
	@rm -vf $(DIR)/$(TARGET)

clean:
	@rm -vf $(OBJ) $(TARGET)
