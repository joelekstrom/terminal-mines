#include "graphics.h"
#include <string.h>

void init_colors() {
	use_default_colors();
	init_pair(COLOR_PAIR_DEFAULT, -1, -1);
	init_pair(COLOR_PAIR_CURSOR, -1, COLOR_GREEN);
	init_pair(COLOR_PAIR_MINE, -1, COLOR_RED);
	init_pair(COLOR_PAIR_FLAG, COLOR_YELLOW, -1);
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

extern struct tm_options global_options;
extern struct minesweeper_tile *adventure_exit_tile;

int tile_index_for_tile(struct minesweeper_game *game, struct minesweeper_tile *tile) {
	// Opened with mine, and game over state where all mines are displayed
	if ((tile->has_mine && game->state == MINESWEEPER_GAME_OVER) || (tile->has_mine && tile->is_opened)) {
		return TILE_INDEX_MINE;
	}

	// In adventure mode, we display the cursor as a player '@' and an exit tile '>'
	if (global_options.adventure_mode) {
		if (tile == adventure_exit_tile) {
			return TILE_INDEX_ADVENTURE_EXIT;
		} else if (tile == game->selected_tile) {
			return TILE_INDEX_ADVENTURE_PLAYER;
		}
	}

	// On opened tile, we show the adjacent mine count
	if (tile->is_opened) {
		return tile->adjacent_mine_count;
	}

	// Flags are shown as an 'F'
	if (tile->has_flag) {
		return TILE_INDEX_FLAG;
	}

	// Unopened tiles
	return TILE_INDEX_UNOPENED;
}

void render_tile(struct minesweeper_game *game, struct minesweeper_tile *tile, WINDOW *window) {
	int index = tile_index_for_tile(game, tile);
	bool is_cursor = tile == game->selected_tile;
	if (index == TILE_INDEX_MINE) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_MINE));
	} else if (tile == adventure_exit_tile) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_ADVENTURE_EXIT));
	} else if (is_cursor && global_options.adventure_mode) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_ADVENTURE_PLAYER));
	} else if (is_cursor) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_CURSOR));
	} else if (index == TILE_INDEX_FLAG) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_FLAG));
	} else if (index >= 1 && index <= 8) {
		wattron(window, COLOR_PAIR(COLOR_PAIR_1 + (index - 1)));
	} else {
		wattron(window, COLOR_PAIR(COLOR_PAIR_DEFAULT));
	}

	// Add 1 to the position in both axes so we stay within the
	// window border.
	unsigned x, y; minesweeper_get_tile_location(game, tile, &x, &y);
	static char *tile_map[] = TILE_MAP;
	mvwaddstr(window, y + 1, x + 1, tile_map[index]);
}

void render_game(struct minesweeper_game *game, WINDOW *window) {
	for (unsigned x = 0; x < game->width; x++) {
		for (unsigned y = 0; y < game->height; y++) {
			struct minesweeper_tile *tile = minesweeper_get_tile_at(game, x, y);
			render_tile(game, tile, window);
		}
	}
}
