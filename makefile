include config.mk

all:
	${CC} ${SRC} ${CFLAGS}-o ${EXE}

clean:
	rm -f ${EXE}

run: all
	./$(EXE)
