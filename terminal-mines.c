#include <minesweeper.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "options.h"
#include "graphics.h"

void start_with_game(struct minesweeper_game *game, struct tm_options options);
void game_loop(WINDOW *window, struct minesweeper_game *game, struct tm_options options);
void tile_changed(struct minesweeper_game *game, struct minesweeper_tile *tile, void *context);

WINDOW *game_win;
WINDOW *status_win;
struct tm_options global_options;
struct minesweeper_tile *adventure_exit_tile;

void setup_ncurses() {
	initscr();
	start_color();
	cbreak();
	noecho();
	init_colors();
	curs_set(0);
}

int main(int argc, char **argv) {
	global_options = parse_options(argc, argv);
	setup_ncurses();

	// Make sure the desired size is not larger than the terminal window
	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	int max_width = screen_width - 2; // Remove 2 so we can fit window border
	int max_height = screen_height - 5; // Window border + status window height

	if (global_options.width > max_width) {
		global_options.width = max_width;
	}

	if (global_options.height > max_height) {
		global_options.height = max_height;
	}

	// Set up a game game
	srand(time(NULL));
	uint8_t *buffer = malloc(minesweeper_minimum_buffer_size(global_options.width, global_options.height));
	struct minesweeper_game *b = minesweeper_init(global_options.width, global_options.height, global_options.mine_density, buffer);
	b->tile_update_callback = &tile_changed;

	// Start the ncurses frontend
	start_with_game(b, global_options);
	free(buffer);
}

void start_with_game(struct minesweeper_game *game, struct tm_options options) {
	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	// Create window where we will draw the game. Add 2
	// tiles padding so we can draw a box around it
	int window_width = game->width + 2;
	int window_height = game->height + 2;
	int window_x = screen_width / 2 - window_width / 2;
	int window_y = screen_height / 2 - window_height / 2;
	game_win = newwin(window_height, window_width, window_y, window_x);
	box(game_win, 0, 0);

	status_win = newwin(3, window_width, window_y + window_height, window_x);

	// When we run in adventure mode, we want to place the player in the
	// bottom left corner, and make sure there are no mines too close
	if (options.adventure_mode) {
		minesweeper_set_cursor(game, 0, game->height - 1);
		adventure_exit_tile = minesweeper_get_tile_at(game, game->width - 1, 0);

		// Clear the 8 tiles closest to the player
		struct minesweeper_tile *clear_tile = minesweeper_get_tile_at(game, 1, game->height - 2);
		if (clear_tile && clear_tile->has_mine) minesweeper_toggle_mine(game, clear_tile);
		struct minesweeper_tile *adjacent_tiles[8]; minesweeper_get_adjacent_tiles(game, clear_tile, adjacent_tiles);
		for (int i = 0; i < 8; i++) {
			struct minesweeper_tile *tile = adjacent_tiles[i];
			if (tile && tile->has_mine) minesweeper_toggle_mine(game, tile);
		}

		// Clear the exit tile
		if (adventure_exit_tile->has_mine) {
			minesweeper_toggle_mine(game, adventure_exit_tile);
		}

		minesweeper_open_tile(game, game->selected_tile);
	} else {
		minesweeper_set_cursor(game, game->width / 2, game->height / 2);
	}

	// Draw an initial representation so you see the window when the game starts
	render_game(game, game_win);
	refresh();
	wrefresh(game_win);
	update_status_window(status_win, game);
	wrefresh(status_win);
	
	game_loop(game_win, game, options);
}

void tm_move_cursor(struct minesweeper_game *game, enum direction direction, struct tm_options options) {
	minesweeper_move_cursor(game, direction, !options.adventure_mode);
	if (options.adventure_mode) {
		if (game->selected_tile == adventure_exit_tile) {
			game->state = MINESWEEPER_WIN;
		} else {
			minesweeper_open_tile(game, game->selected_tile);
		}
	}
}

void game_loop(WINDOW *window, struct minesweeper_game *game, struct tm_options options) {
	// Wait for keygame input
	keypad(stdscr, TRUE);
	int ch;
	while((ch = getch()) != KEY_F(1) && (game->state == MINESWEEPER_PENDING_START || game->state == MINESWEEPER_PLAYING)) {
		struct minesweeper_tile *previous_tile = game->selected_tile;
		switch(ch) {	
		case KEY_LEFT:
		case 'h':
			tm_move_cursor(game, LEFT, options);
			break;

		case KEY_RIGHT:
		case 'l':
			tm_move_cursor(game, RIGHT, options);
			break;

		case KEY_UP:
		case 'k':
			tm_move_cursor(game, UP, options);
			break;

		case KEY_DOWN:
		case 'j':
			tm_move_cursor(game, DOWN, options);
			break;

		case 'g':
		case 'f':
			minesweeper_toggle_flag(game, game->selected_tile);
			update_status_window(status_win, game);
			break;

		case ',':
			minesweeper_open_tile(game, game->selected_tile);
			update_status_window(status_win, game);
			break;
		}

		if (game->selected_tile != previous_tile) {
			render_tile(game, previous_tile, window);
			render_tile(game, game->selected_tile, window);
		}

		wrefresh(window);
	}

	render_game(game, window);
	wrefresh(window);

	// Create a win/lose window, and then exit the game after a key press
	char *end_text = NULL;
	if (game->state == MINESWEEPER_GAME_OVER) {
		end_text = "You failed!";
	} else if (game->state == MINESWEEPER_WIN) {
		end_text = "You win! Niiiice!";
	}

	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);
	int window_width = strlen(end_text) + 2;
	WINDOW *end_win = newwin(3, window_width, screen_height / 2 - 2, screen_width / 2 - window_width / 2);
	box(end_win, 0, 0);
	mvwprintw(end_win, 1, 1, end_text);
	wrefresh(end_win);

	wgetch(window);
 	endwin();
}

// Callback from libminesweeper
void tile_changed(struct minesweeper_game *game, struct minesweeper_tile *tile, void *context) {
	render_tile(game, tile, game_win);
}
