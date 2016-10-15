#include <minesweeper.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "options.h"
#include "graphics.h"

void start_with_board(struct board *board, struct tm_options options);
void game_loop(WINDOW *window, struct board *board, struct tm_options options);
void tile_changed(struct board *board, uint8_t *tile, int x, int y);

WINDOW *board_win;
WINDOW *status_win;
struct tm_options global_options;

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

	// Set up a game board
	srand(time(NULL));
	uint8_t *buffer = malloc(minimum_buffer_size(global_options.width, global_options.height));
	struct board *b = board_init(global_options.width, global_options.height, global_options.mine_density, buffer);
	b->on_tile_updated = &tile_changed;

	// Start the ncurses frontend
	start_with_board(b, global_options);
	free(buffer);
}

void start_with_board(struct board *board, struct tm_options options) {
	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	// Create window where we will draw the board. Add 2
	// tiles padding so we can draw a box around it
	int window_width = board->_width + 2;
	int window_height = board->_height + 2;
	int window_x = screen_width / 2 - window_width / 2;
	int window_y = screen_height / 2 - window_height / 2;
	board_win = newwin(window_height, window_width, window_y, window_x);
	box(board_win, 0, 0);

	status_win = newwin(3, window_width, window_y + window_height, window_x);

	// When we run in adventure mode, we want to place the player in the
	// bottom left corner, and make sure there are no mines too close
	if (options.adventure_mode) {
		board->cursor_x = 0;
		board->cursor_y = board->_height - 1;
		options.adventure_exit_tile = get_tile_at(board, board->_width - 1, 0);
		open_tile_at_cursor(board);
	}

	// Draw an initial representation so you see the window when the game starts
	render_board(board, board_win, options);
	refresh();
	wrefresh(board_win);
	update_status_window(status_win, board);
	wrefresh(status_win);
	
	game_loop(board_win, board, options);
}

void tm_move_cursor(struct board *board, enum direction direction, struct tm_options options) {
	move_cursor(board, direction, !options.adventure_mode);
	if (options.adventure_mode) {
		uint8_t *tile = get_tile_at(board, board->cursor_x, board->cursor_y);
		if (tile == options.adventure_exit_tile) {
			board->_state = BOARD_WIN;
		} else {
			open_tile_at_cursor(board);
		}
	}
}

void game_loop(WINDOW *window, struct board *board, struct tm_options options) {
	// Wait for keyboard input
	keypad(stdscr, TRUE);
	int ch;
	while((ch = getch()) != KEY_F(1) && (board->_state == BOARD_PENDING_START || board->_state == BOARD_PLAYING)) {
		int previous_x = board->cursor_x;
		int previous_y = board->cursor_y;
		switch(ch) {	
		case KEY_LEFT:
		case 'h':
			tm_move_cursor(board, LEFT, options);
			break;

		case KEY_RIGHT:
		case 'l':
			tm_move_cursor(board, RIGHT, options);
			break;

		case KEY_UP:
		case 'k':
			tm_move_cursor(board, UP, options);
			break;

		case KEY_DOWN:
		case 'j':
			tm_move_cursor(board, DOWN, options);
			break;

		case 'g':
		case 'f':
			toggle_flag_at_cursor(board);
			update_status_window(status_win, board);
			break;

		case ',':
			open_tile_at_cursor(board);
			update_status_window(status_win, board);
			break;
		}

		if (board->cursor_x != previous_x || board->cursor_y != previous_y) {
			render_tile(board, window, previous_x, previous_y, options);
			render_tile(board, window, board->cursor_x, board->cursor_y, options);
		}

		wrefresh(window);
	}

	render_board(board, window, options);
	wrefresh(window);

	// Create a win/lose window, and then exit the game after a key press
	char *end_text = NULL;
	if (board->_state == BOARD_GAME_OVER) {
		end_text = "You failed!";
	} else if (board->_state == BOARD_WIN) {
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
void tile_changed(struct board *board, uint8_t *tile, int x, int y) {
	render_tile(board, board_win, x, y, global_options);
}

