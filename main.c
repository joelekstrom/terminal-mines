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
	char text[] = "HELURRRRRR!!!";

	// Create the game board
	struct board b;
	board_init(&b, 10, 10, 0.2);

	// Set up colors
	init_pair(1, COLOR_YELLOW, COLOR_GREEN); // Cursors

	// Make sure this game is actually interesting
	srand(time(NULL));
	
	// Create window where we will draw the board
	WINDOW *board_win = newwin(b.height, b.width, screen_height / 2 - b.height / 2, screen_width / 2 - b.width / 2);
	box(board_win, 0, 0);
	render(board_win, &b);
	refresh();

	// Listen for keyboard input
	keypad(stdscr, TRUE);
	int ch;
	while((ch = getch()) != KEY_F(1)) {	
		switch(ch) {	
			case KEY_LEFT:
			case 'h':
				b.cursor_x -= 1;
				break;

			case KEY_RIGHT:
			case 'l':
				b.cursor_x += 1;
				break;

			case KEY_UP:
			case 'k':
				b.cursor_y -= 1;
				break;

			case KEY_DOWN:
		    case 'j':
				b.cursor_y += 1;
				break;

		    case ',':
				open_tile(&b);
				break;
		}
		render(board_win, &b);
		wrefresh(board_win);
	}

 	wgetch(board_win);
 	endwin();
  	return 0;
}
