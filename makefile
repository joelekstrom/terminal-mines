CFLAGS = -std=c99 -Wall
LDLIBS = -lncurses -lminesweeper
LDFLAGS = -Llibminesweeper
CPPFLAGS = -Ilibminesweeper/include

INSTALL_PATH?=/usr/local
EXECUTABLE = terminal-mines
LIBMINESWEEPER = libminesweeper/libminesweeper.a

$(EXECUTABLE): $(LIBMINESWEEPER) terminal-mines.o options.o graphics.o

$(LIBMINESWEEPER):
	$(MAKE) -C libminesweeper

.PHONY: clean install
clean:
	rm -f $(EXECUTABLE)
	$(MAKE) -C libminesweeper clean

install:
	install -d $(INSTALL_PATH)/bin/
	install $(EXECUTABLE) $(INSTALL_PATH)/bin/
	install -d $(INSTALL_PATH)/share/man/man1/
	install man/$(EXECUTABLE).1 $(INSTALL_PATH)/share/man/man1/
