sweeper: main.c game_logic/* frontends/*
	clang main.c game_logic/board.c frontends/curses.c -Igame_logic -Ifrontends -o sweeper -lncurses
