#include <minunit.h>
#include <stdio.h>
#include <board.h>

int tests_run = 0;
struct board board;

static char * test_init() {
	board.data = NULL;
	board_init(&board, 100, 100, 0.1);
    mu_assert("Error: init must allocate board data.", board.data != NULL);
	mu_assert("Error: mines must not be placed until the first tile is opened.", board.mines_placed == false);
	mu_assert("Error: cursor should be centered after init", board.cursor_x == 50 && board.cursor_y == 50);
   	return 0;
}

static char * test_open_first_tile() {
	open_tile_at_cursor(&board);
	mu_assert("Error: mines must be placed after first tile is opened", board.mines_placed == true);
	mu_assert("Error: there must not be a mine under the first opened tile", board.game_over == false);
	return 0;
}

static char * test_open_mine() {
	// Let's find a tile with a mine and open it...
	for (int x = 0; x < board.width; x++) {
		for (int y = 0; y < board.height; y++) {
			uint8_t *tile = get_tile_at(&board, x, y);
			if (*tile & TILE_MINE) {
				board.cursor_x = x;
				board.cursor_y = y;
				open_tile_at_cursor(&board);
				mu_assert("Error: After opening a mine tile, game_over must be true.", board.game_over == true);
				break;
			}
		}
	}
	return 0;
}

static char * all_tests() {
	mu_run_test(test_init);
	mu_run_test(test_open_first_tile);
	mu_run_test(test_open_mine);
	return 0;
}
 
int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	return 0;
}
