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
	mu_assert("Error: Cursor should be centered after init", board.cursor_x == 50 && board.cursor_y == 50);
   	return 0;
}

static char * all_tests() {
	mu_run_test(test_init);
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
