#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum {
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
	float mine_density;
	int cursor_x;
	int cursor_y;
	bool game_over;
	bool mines_placed;
	uint8_t *data;
};

void board_init(struct board* board, int width, int height, float mine_density);
void board_deinit(struct board* board);
void move_cursor(struct board* board, enum direction direction);
void open_tile_at_cursor(struct board* board);
void toggle_flag_at_cursor(struct board* board);
uint8_t* get_tile_at(struct board *board, int x, int y);
void get_adjacent_tiles(struct board *board, uint8_t *tile, uint8_t **adjacent_tiles);
uint8_t adjacent_mine_count(uint8_t* tile);

#endif
