#include "board.h"
#include <stdlib.h>

void set_tile(struct board *board, int x, int y, uint8_t data);
uint8_t get_tile(struct board *board, int x, int y);

void board_init(struct board *board, int width, int height, float mine_density) {
	board->width = width;
	board->height = height;
	board->cursor_x = 1;
	board->cursor_y = 1;
	board->data = calloc(sizeof(uint8_t), width * height);

	// Generate mines
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			float r = (float)rand() / (float)RAND_MAX;
			if (r < mine_density) {
				set_tile(board, x, y, TILE_MINE);
			} else {
				set_tile(board, x, y, 0);
			}
		}
	}
}

void board_deinit(struct board *board) {
	free(board->data);
}

void open_tile(struct board *board) {
	uint8_t tile_data = get_tile(board, board->cursor_x, board->cursor_y);
	set_tile(board, board->cursor_x, board->cursor_y, tile_data | TILE_OPENED);
}

uint8_t get_tile(struct board *board, int x, int y) {
	return board->data[board->width * y + x];
}

void set_tile(struct board *board, int x, int y, uint8_t data) {
	board->data[board->width * y + x] = data;
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
	uint8_t tile_data = get_tile(board, x, y);
	switch (tile_data) {
	    case TILE_OPENED | TILE_MINE:
			return '*';

	    case TILE_OPENED | TILE_FLAG:
		    return 'F';

	    case TILE_OPENED:
			return '.';
			
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
