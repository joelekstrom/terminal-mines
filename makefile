C_FLAGS = -std=c99 -Wall

executable = bin/terminal-mines
library = bin/libminesweeper.a

$(executable): $(library) frontends/ncurses/*.c
	$(CC) $(C_FLAGS) frontends/ncurses/*.c -Ilib -Ifrontends/ncurses -Lbin -o $@ -lncurses -lminesweeper

$(library): lib/board.c
	mkdir -p bin
	$(CC) $(C_FLAGS) -c lib/board.c -o bin/board.o
	ar rcs $@ bin/board.o
	rm bin/board.o

.PHONY: test, clean
test: bin/test
	bin/test

bin/test: $(library)
	$(CC) $(C_FLAGS) tests/board_tests.c -Ilib -Itests -Lbin -lminesweeper -o $@

clean:
	rm -rf bin/
