#import <ncurses.h>
#import <stdio.h>
#import <string.h>
#import "board.h"

int main(int argc, char **argv) {
	initscr();
	start_color();
	cbreak();

	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);		/* get the number of rows and columns */

	char text[] = "HELURRRRRR!!!";

	// Create the game board
	struct board b;
	board_init(&b, 10, 10);

	// Set up colors
	init_pair(1, COLOR_YELLOW, COLOR_GREEN); // Cursors
	
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
				b.cursor_x -= 1;
				break;

			case KEY_RIGHT:
				b.cursor_x += 1;
				break;

			case KEY_UP:
				b.cursor_y -= 1;
				break;

			case KEY_DOWN:
				b.cursor_y += 1;
				break;
		}
		render(board_win, &b);
		wrefresh(board_win);
	}

 	wgetch(board_win);
 	endwin();
  	return 0;
}
