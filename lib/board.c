#include "board.h"

#define MAX(A, B) A > B ? A : B
#define MIN(A, B) A < B ? A : B

uint8_t* get_tile_at(struct board *board, int x, int y);
void open_tile(uint8_t* tile);

void board_init(struct board *board, int width, int height, float mine_density) {
	board->width = width;
	board->height = height;
	board->cursor_x = width / 2;
	board->cursor_y = height / 2;
	board->mine_density = mine_density;
	board->mines_placed = false;
	board->game_over = false;
	board->data = calloc(sizeof(uint8_t), width * height);
}

bool is_out_of_bounds(struct board *b, int x, int y) {
	return x < 0 || x >= b->width || y < 0 || y >= b->height;
}

/**
 * Get a pointer to a tile. Contains bounds checking and
 * will return NULL if the tile is out of bounds. This
 * is to simplify code that enumerates "adjacent" tiles.
 */
uint8_t* get_tile_at(struct board *board, int x, int y) {
	if (is_out_of_bounds(board, x, y))
		return NULL;
	return &board->data[board->width * y + x];
}

void get_adjacent_tiles(struct board *board, uint8_t *tile, uint8_t **adjacent_tiles) {
	int tile_index = tile - board->data;
	int y = tile_index / board->width;
	int x = tile_index % board->width;
	adjacent_tiles[0] = get_tile_at(board, x - 1, y - 1);
	adjacent_tiles[1] = get_tile_at(board, x - 1, y);
	adjacent_tiles[2] = get_tile_at(board, x - 1, y + 1);
	adjacent_tiles[3] = get_tile_at(board, x, y - 1);
	adjacent_tiles[4] = get_tile_at(board, x, y + 1);
	adjacent_tiles[5] = get_tile_at(board, x + 1, y - 1);
	adjacent_tiles[6] = get_tile_at(board, x + 1, y);
	adjacent_tiles[7] = get_tile_at(board, x + 1, y + 1);
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
	*tile = shifted_value | (*tile & 0x0F);
}

void place_mine(struct board *board, uint8_t *tile) {
	*tile |= TILE_MINE;

	// Increase the mine counts on all adjacent tiles
	uint8_t *adjacent_tiles[8];
	get_adjacent_tiles(board, tile, adjacent_tiles);
	for (int i = 0; i < 8; i++) {
		uint8_t *adjacent_tile = adjacent_tiles[i];
		if (adjacent_tile) {
			increment_adjacent_mine_count(adjacent_tile);
		}
	}
}

void generate_mines(struct board *board, uint8_t *safe_tile) {
	long tile_count = board->width * board->height;
	long mine_count = tile_count * board->mine_density;
	for (long i = 0; i < mine_count; i++) {
		float r = (float)rand() / (float)RAND_MAX;
		long random_index = r * (tile_count - 1);
		uint8_t *random_tile = &board->data[random_index];
		if (random_tile != safe_tile) {
			place_mine(board, random_tile);
		}
	}
}

void board_deinit(struct board *board) {
	free(board->data);
}

void open_adjacent_tiles(struct board *board, uint8_t *tile) {
	uint8_t *adjacent_tiles[8];
	get_adjacent_tiles(board, tile, adjacent_tiles);
	for (int i = 0; i < 8; i++) {
		uint8_t *adjacent_tile = adjacent_tiles[i];
		if (adjacent_tile && !(*adjacent_tile & TILE_OPENED) && !(*adjacent_tile & TILE_FLAG)) {
			open_tile(adjacent_tile);
			if (*adjacent_tile & TILE_MINE) {
				board->game_over = true;
				return;
			}

			if (adjacent_mine_count(adjacent_tile) == 0) {
				open_adjacent_tiles(board, adjacent_tile);
			}
		}
	}
}

void open_tile_at_cursor(struct board *board) {
	uint8_t* tile = get_tile_at(board, board->cursor_x, board->cursor_y);

	if (!board->mines_placed) {
		generate_mines(board, tile);
		board->mines_placed = true;
	}

	// If this tile is already opened and has a mine count,
	// it should open all adjacent tiles instead. This mimics
	// the behaviour in the original minesweeper where you can
	// right click opened tiles to open adjacent tiles quickly.
	if (*tile & TILE_OPENED && adjacent_mine_count(tile) > 0) {
		open_adjacent_tiles(board, tile);
		return;
	}

	if (*tile & TILE_FLAG) {
		return;
	}

	open_tile(tile);

	if (*tile & TILE_MINE) {
		board->game_over = true;
	} else if (adjacent_mine_count(tile) == 0) {
		open_adjacent_tiles(board, tile);
	}
}

void toggle_flag_at_cursor(struct board *board) {
	uint8_t* tile = get_tile_at(board, board->cursor_x, board->cursor_y);
	*tile ^= TILE_FLAG;
}

void open_tile(uint8_t *tile) {
	*tile |= TILE_OPENED;
}

void move_cursor(struct board *board, enum direction direction) {
	switch (direction) {
	case LEFT:
		board->cursor_x = MAX(board->cursor_x - 1, 0);
		break;
	case RIGHT:
		board->cursor_x = MIN(board->cursor_x + 1, board->width - 1);
		break;
	case UP:
		board->cursor_y = MAX(board->cursor_y - 1, 0);
		break;
	case DOWN:
		board->cursor_y = MIN(board->cursor_y + 1, board->height - 1);
		break;
	}
}
