#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include <minesweeper.h>
#include "options.h"

enum {
	COLOR_PAIR_DEFAULT = 1,
	COLOR_PAIR_CURSOR,
	COLOR_PAIR_MINE,
	COLOR_PAIR_FLAG,
	COLOR_PAIR_1,
	COLOR_PAIR_2,
	COLOR_PAIR_3,
	COLOR_PAIR_4,
	COLOR_PAIR_5,
	COLOR_PAIR_6,
	COLOR_PAIR_7,
	COLOR_PAIR_8,
	COLOR_PAIR_ADVENTURE_EXIT,
	COLOR_PAIR_ADVENTURE_PLAYER
};

enum {
	CHAR_MINE = '*',
	CHAR_ADVENTURE_EXIT = '>',
	CHAR_ADVENTURE_PLAYER = '@',
	CHAR_FLAG = 'F',
	CHAR_TILE = '#',
	CHAR_EMPTY = ' '
};

void init_colors();
void render_game(struct minesweeper_game *game, WINDOW *window);
void render_tile(struct minesweeper_game *game, struct minesweeper_tile *tile, WINDOW *window);
void update_status_window(WINDOW *status_window, struct minesweeper_game *game);

#endif
