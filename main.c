#include <time.h>
#include <board.h>
#include <curses.h>

int main(int argc, char **argv) {
	// Set up a game board
	srand(time(NULL));
	struct board b;
	board_init(&b, 20, 10, 0.1);

	// Start the ncurses frontend
	start_with_board(&b);
}
