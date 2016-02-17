#include "board.h"
#include <stdlib.h>
#include <stdbool.h>

uint8_t* get_tile(struct board *board, int x, int y);
void generate_mines(struct board *board, float mine_density);

void board_init(struct board *board, int width, int height, float mine_density) {
	board->width = width;
	board->height = height;
	board->cursor_x = 1;
	board->cursor_y = 1;
	board->data = calloc(sizeof(uint8_t), width * height);
	generate_mines(board, mine_density);
}

/**
 * We use the last 4 bits of a tile for tile data such as
 * opened, mine, flag etc. the first 4 stores a count of
 * adjacent mines. This function gets that value.
 */
uint8_t adjacent_mine_count(uint8_t* tile) {
	return (*tile & 0xF0) >> 4;
}

void increment_adjacent_mine_count(uint8_t* tile) {
	uint8_t value = adjacent_mine_count(tile) + 1;
	uint8_t shifted_value = value << 4;
	uint8_t final_value = shifted_value | (*tile & 0x0F);
	*tile = final_value;
}

void place_mine(struct board *board, int x, int y) {
	uint8_t* tile = get_tile(board, x, y);
	*tile |= TILE_MINE;

	// Enumerate all adjacent tiles and increase their
	// "adjacent mine count" by one
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			uint8_t* adjacent_tile = get_tile(board, i, j);
			if (adjacent_tile && adjacent_tile != tile) {
				increment_adjacent_mine_count(adjacent_tile);
			}
		}
	}
}

void generate_mines(struct board *board, float mine_density) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			float r = (float)rand() / (float)RAND_MAX;
			if (r < mine_density) {
				place_mine(board, x, y);
			}
		}
	}
}

void board_deinit(struct board *board) {
	free(board->data);
}

void open_tile(struct board *board) {
	uint8_t* tile = get_tile(board, board->cursor_x, board->cursor_y);
	*tile |= TILE_OPENED;
}

bool is_out_of_bounds(struct board *b, int x, int y) {
	return x < 0 || x >= b->width || y < 0 || y >= b->height;
}

/**
 * Get a pointer to a tile. Contains bounds checking and
 * will return NULL if the tile is out of bounds. This
 * is to simplify code that enumerates "adjacent" tiles.
 */
uint8_t* get_tile(struct board *board, int x, int y) {
	if (is_out_of_bounds(board, x, y))
		return NULL;
	return &board->data[board->width * y + x];
}

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

void move_cursor(struct board *board, enum direction direction) {
	switch (direction) {
	case LEFT:
		board->cursor_x = max(board->cursor_x - 1, 0);
		break;
	case RIGHT:
		board->cursor_x = min(board->cursor_x + 1, board->width - 1);
		break;
	case UP:
		board->cursor_y = max(board->cursor_y - 1, 0);
		break;
	case DOWN:
		board->cursor_y = min(board->cursor_y + 1, board->height - 1);
		break;
	}
}

char char_at_tile(struct board *board, int x, int y) {
	uint8_t *tile = get_tile(board, x, y);
	switch (*tile & 0x0F) {
	case TILE_OPENED | TILE_MINE:
		return '*';

	case TILE_FLAG:
		return 'F';

	case TILE_OPENED: {
		uint8_t adj_count = adjacent_mine_count(tile);
		if (adj_count > 0) {
			return '0' + adj_count;
		}
		return '.';
	}

	default:
		return '#';
	}
}

void render(WINDOW *window, struct board *board) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
   			char sprite = char_at_tile(board, x, y);			
			int is_cursor = (x == board->cursor_x && y == board->cursor_y) ? 1 : 0;
			if (is_cursor) {
				wattron(window, COLOR_PAIR(1));
			}
			
			mvwaddch(window, y + 1, x + 1, sprite);

			if (is_cursor) {
				wattroff(window, COLOR_PAIR(1));
			}
		}
	}
}
