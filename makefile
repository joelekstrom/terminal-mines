C_FLAGS = -std=c99 -Wall

bin/sweeper: bin main.c game_logic/* frontends/*
	$(CC) $(C_FLAGS) main.c game_logic/board.c frontends/curses.c -Igame_logic -Ifrontends -o bin/sweeper -lncurses

bin:
	mkdir bin

.PHONY: test, clean
test: bin
	$(CC) $(C_FLAGS) tests/board_tests.c game_logic/board.c -Igame_logic -Itests -o bin/test
	bin/test

clean:
	rm -rf bin/
