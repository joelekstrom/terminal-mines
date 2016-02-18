#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef BOARD_H
#define BOARD_H

enum tile_info {
	TILE_OPENED = 1,
	TILE_MINE = 1 << 1,
	TILE_FLAG = 1 << 2
};

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct board {
	int width;
	int height;
	int cursor_x;
	int cursor_y;
	bool game_over;
	uint8_t *data;
};

void board_init(struct board *board, int width, int height, float mine_density);
void board_deinit(struct board *board);
void render(WINDOW *window, struct board *board);
void move_cursor(struct board *board, enum direction direction);
void open_tile(struct board *board);
void toggle_flag(struct board *board);

#endif
