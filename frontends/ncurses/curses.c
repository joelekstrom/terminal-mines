#include <curses.h>
#include <ncurses.h>

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
	COLOR_PAIR_8
};

void init_colors();
void start_with_board(struct board *board);
char char_at_tile(struct board *board, int x, int y);
void game_loop(WINDOW *window, struct board *board);
void render_board(struct board *board, WINDOW *window);

void start_with_board(struct board *board) {
	initscr();
	start_color();
	cbreak();
	noecho();
	init_colors();

	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	// Create window where we will draw the board. Add 2
	// tiles padding so we can draw a box around it
	int window_width = board->width + 2;
	int window_height = board->height + 2;
	WINDOW *board_win = newwin(window_height, window_width, screen_height / 2 - window_height / 2, screen_width / 2 - window_width / 2);
	box(board_win, 0, 0);

	// Draw an initial representation so you see the window when the game starts
	render_board(board, board_win);
	refresh();
	wrefresh(board_win);
	
	game_loop(board_win, board);
}

void game_loop(WINDOW *window, struct board *board) {
	// Wait for keyboard input
	keypad(stdscr, TRUE);
	int ch;
	while((ch = getch()) != KEY_F(1) && !board->game_over) {
		switch(ch) {	
		case KEY_LEFT:
		case 'h':
			move_cursor(board, LEFT);
			break;

		case KEY_RIGHT:
		case 'l':
			move_cursor(board, RIGHT);
			break;

		case KEY_UP:
		case 'k':
			move_cursor(board, UP);
			break;

		case KEY_DOWN:
		case 'j':
			move_cursor(board, DOWN);
			break;

		case 'g':
		case 'f':
			toggle_flag_at_cursor(board);
			break;

		case ',':
			open_tile_at_cursor(board);
			break;
		}

		render_board(board, window);
		wrefresh(window);
	}

 	wgetch(window);
 	endwin();
}

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
}

char char_at_tile(struct board *board, int x, int y) {
	uint8_t* tile = get_tile_at(board, x, y);
	if ((*tile & TILE_MINE) && board->game_over) {
		if (board->game_over) {
			return '*';
		}
	} else if (*tile & TILE_OPENED) {
		uint8_t adj_count = adjacent_mine_count(tile);
		if (adj_count > 0) {
			return '0' + adj_count;
		}
		return '.';
	} else if (*tile & TILE_FLAG) {
		return 'F';
	}
	return '#';
}

void render_board(struct board *board, WINDOW *window) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
   			char sprite = char_at_tile(board, x, y);			
			int is_cursor = (x == board->cursor_x && y == board->cursor_y) ? 1 : 0;
			if (is_cursor) {
				wattron(window, COLOR_PAIR(COLOR_PAIR_CURSOR));
			} else if (sprite == '*') {
				wattron(window, COLOR_PAIR(COLOR_PAIR_MINE));
			} else if (sprite == 'F') {
				wattron(window, COLOR_PAIR(COLOR_PAIR_FLAG));
			} else if (sprite >= '1' && sprite <= '8') {
				wattron(window, COLOR_PAIR(COLOR_PAIR_1 + (sprite - '1')));
			} else {
				wattron(window, COLOR_PAIR(COLOR_PAIR_DEFAULT));
			}

			// Add 1 to the position in both axes so we stay within the
			// window border.
			mvwaddch(window, y + 1, x + 1, sprite);
		}
	}
}
