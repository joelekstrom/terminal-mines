bin/sweeper: bin main.c game_logic/* frontends/*
	clang main.c game_logic/board.c frontends/curses.c -Igame_logic -Ifrontends -o bin/sweeper -lncurses

bin:
	mkdir bin

.PHONY: test, clean
test: bin
	clang tests/board_tests.c game_logic/board.c -Igame_logic -Itests -o bin/test
	bin/test

clean:
	rm -rf bin/
