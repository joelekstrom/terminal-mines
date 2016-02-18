#import <ncurses.h>
#import <stdio.h>
#import <string.h>
#import <stdlib.h>
#import <time.h>
#import "board.h"

int main(int argc, char **argv) {
	initscr();
	start_color();
	cbreak();
	noecho();

	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	// Create the game board
	srand(time(NULL));
	struct board b;
	board_init(&b, 20, 10, 0.1);

	// Set up colors
	use_default_colors();
	init_pair(TERMINE_COLOR_DEFAULT, -1, -1);
	init_pair(TERMINE_COLOR_CURSOR, -1, COLOR_GREEN);
	init_pair(TERMINE_COLOR_MINE, -1, COLOR_RED);
	init_pair(TERMINE_COLOR_FLAG, COLOR_YELLOW, -1);
	init_pair(TERMINE_COLOR_1, COLOR_BLUE, -1);
	init_pair(TERMINE_COLOR_2, COLOR_GREEN, -1);
	init_pair(TERMINE_COLOR_3, COLOR_RED, -1);
	init_pair(TERMINE_COLOR_4, COLOR_YELLOW, -1);
	// TODO: Set colors for 5,6,7,8
	
	// Create window where we will draw the board. Add 2
	// to the window size so we can draw a box around it
	int window_width = b.width + 2;
	int window_height = b.height + 2;
	WINDOW *board_win = newwin(window_height, window_width, screen_height / 2 - window_height / 2, screen_width / 2 - window_width / 2);
	box(board_win, 0, 0);
	render(board_win, &b);
	refresh();

	// Listen for keyboard input
	keypad(stdscr, TRUE);
	int ch;
	while((ch = getch()) != KEY_F(1) && !b.game_over) {
		switch(ch) {	
		case KEY_LEFT:
		case 'h':
			move_cursor(&b, LEFT);
			break;

		case KEY_RIGHT:
		case 'l':
			move_cursor(&b, RIGHT);
			break;

		case KEY_UP:
		case 'k':
			move_cursor(&b, UP);
			break;

		case KEY_DOWN:
		case 'j':
			move_cursor(&b, DOWN);
			break;

		case 'g':
		case 'f':
			toggle_flag_at_cursor(&b);
			break;

		case ',':
			open_tile_at_cursor(&b);
			break;
		}

		render(board_win, &b);
		wrefresh(board_win);
	}

 	wgetch(board_win);
 	endwin();
  	return 0;
}
