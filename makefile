C_FLAGS = -std=c99 -Wall

INSTALL_PATH?=/usr/local
EXECUTABLE = terminal-mines
LIBMINESWEEPER = libminesweeper/libminesweeper.a

$(EXECUTABLE): $(LIBMINESWEEPER) terminal-mines.c options.c
	$(CC) $(C_FLAGS) *.c -Ilibminesweeper/include -Llibminesweeper -o $@ -lncurses -lminesweeper

$(LIBMINESWEEPER):
	$(MAKE) -C libminesweeper

.PHONY: clean install
clean:
	rm -f $(EXECUTABLE)
	$(MAKE) -C libminesweeper clean

install:
	install ./$(EXECUTABLE) $(INSTALL_PATH)/bin/
	install ./man/$(EXECUTABLE).1 $(INSTALL_PATH)/share/man/man1/
