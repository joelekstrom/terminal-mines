C_FLAGS = -std=c99 -Wall

executable = terminal-mines
libminesweeper = libminesweeper/libminesweeper.a

$(executable): $(libminesweeper)
	$(CC) $(C_FLAGS) *.c -Ilibminesweeper/include -Llibminesweeper -o $@ -lncurses -lminesweeper

$(libminesweeper):
	$(MAKE) -C libminesweeper

clean:
	rm -f $(executable)
	$(MAKE) -C libminesweeper clean
