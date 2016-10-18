#include "graphics.h"
#include <string.h>

void init_colors() {
	use_default_colors();
	init_pair(COLOR_PAIR_DEFAULT, -1, -1);
	init_pair(COLOR_PAIR_CURSOR, -1, COLOR_GREEN);
	init_pair(COLOR_PAIR_MINE, -1, COLOR_RED);
	init_pair(COLOR_PAIR_FLAG, COLOR_WHITE, COLOR_YELLOW);
	init_pair(COLOR_PAIR_1, COLOR_BLUE, -1);
	init_pair(COLOR_PAIR_2, COLOR_GREEN, -1);
	init_pair(COLOR_PAIR_3, COLOR_RED, -1);
	init_pair(COLOR_PAIR_4, COLOR_YELLOW, -1);
	// TODO: Set colors for 5,6,7,8
	init_pair(COLOR_PAIR_ADVENTURE_EXIT, COLOR_BLACK, COLOR_GREEN);
	init_pair(COLOR_PAIR_ADVENTURE_PLAYER, COLOR_GREEN, -1);
}

void update_status_window(WINDOW *status_window, struct minesweeper_game *game) {
	int window_width = game->width;
	
	char mine_text[128];
	char flag_text[128];
	snprintf(mine_text, sizeof(mine_text), "Mines: %d", game->mine_count);
	snprintf(flag_text, sizeof(flag_text), "Flags: %d", game->flag_count);

	int flag_text_length = strlen(flag_text);

	wclear(status_window);
	box(status_window, 0, 0);
	mvwprintw(status_window, 1, 1, "%s", mine_text);
	mvwprintw(status_window, 1, window_width - (flag_text_length - 1), "%s", flag_text);
	wrefresh(status_window);
}

char char_for_tile(struct minesweeper_game *game, uint8_t *tile, struct tm_options options) {
	// Opened with mine, and game over state where all mines are displayed
	if (((*tile & TILE_MINE) && game->state == MINESWEEPER_GAME_OVER) || (*tile & TILE_MINE && *tile & TILE_OPENED)) {
		return CHAR_MINE;
	}

	// In adventure mode, we display the cursor as a player '@' and an exit tile '>'
	if (options.adventure_mode) {
		if (tile == options.adventure_exit_tile) {
			return CHAR_ADVENTURE_EXIT;
		} else if (tile == game->selected_tile) {
			return CHAR_ADVENTURE_PLAYER;
		}
	}

	// On opened tile, we show the adjacent mine count
	if (*tile & TILE_OPENED) {
		uint8_t adj_count = minesweeper_get_adjacent_mine_count(tile);
		if (adj_count > 0) {
			return '0' + adj_count;
		}
		return CHAR_EMPTY;
	}

	// Flags are shown as an 'F'
	if (*tile & TILE_FLAG) {
		return CHAR_FLAG;
	}

	// Unopened tiles
	return CHAR_TILE;
}

void render_tile(struct minesweeper_game *game, uint8_t *tile, WINDOW *window,  struct tm_options options) {
	char c = char_for_tile(game, tile, options);
	bool is_cursor = (tile == game->selected_tile) ? true : false;
	if (c == CHAR_MINE) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_MINE));
	} else if (tile == options.adventure_exit_tile) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_ADVENTURE_EXIT));
	} else if (is_cursor && options.adventure_mode) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_ADVENTURE_PLAYER));
	} else if (is_cursor) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_CURSOR));
	} else if (c == CHAR_FLAG) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_FLAG));
	} else if (c >= '1' && c <= '8') {
		wattron(window, COLOR_PAIR(COLOR_PAIR_1 + (c - '1')));
	} else {
		wattron(window, COLOR_PAIR(COLOR_PAIR_DEFAULT));
	}

	// Add 1 to the position in both axes so we stay within the
	// window border.
	unsigned x, y; minesweeper_get_tile_location(game, tile, &x, &y);
	mvwaddch(window, y + 1, x + 1, c);
}

void render_game(struct minesweeper_game *game, WINDOW *window, struct tm_options options) {
	for (unsigned x = 0; x < game->width; x++) {
		for (unsigned y = 0; y < game->height; y++) {
			uint8_t *tile = minesweeper_get_tile_at(game, x, y);
			render_tile(game, tile, window, options);
		}
	}
}
