#include "board.h"

void board_init(struct board *board, int width, int height) {
	board->width = width;
	board->height = height;
	board->cursor_x = 1;
	board->cursor_y = 1;
	board->data = malloc(sizeof(int) * width * height);
	return;

	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			board->data[x][y] = UNOPENED;
		}
	}
}

void render(WINDOW *window, struct board *board) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			int data = UNOPENED;//board->data[x][y];

			char c;
			switch (data) {
				case FLAG:
					c = 'F';
					break;

				case MINE:
					c = '*';
					break;

				case UNOPENED:
					c = '#';
					break;

				case NOTHING:
					c = '.';
					break;

				default:
					c = 48 + data; // Render the number
					break;
			}

			int is_cursor = (x == board->cursor_x && y == board->cursor_y) ? 1 : 0;
			if (is_cursor) {
				wattron(window, COLOR_PAIR(1));
			}
			
			mvwaddch(window, y, x, c);

			if (is_cursor) {
				wattroff(window, COLOR_PAIR(1));
			}
		}
	}
}