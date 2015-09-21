#include <ncurses.h>
#include <stdlib.h>

#ifndef BOARD_H
#define BOARD_H

enum {
	FLAG = 100, 
	MINE, 
	UNOPENED,
	NOTHING
};

struct board {
	int width;
	int height;
	int cursor_x;
	int cursor_y;
	int **data;
};

void board_init(struct board *board, int width, int height);
void render(WINDOW *window, struct board *board);

#endif