#import <stdbool.h>

struct tm_options {
	int width;
	int height;
	float mine_density;
};

struct tm_options parse_options(int argc, char **argv);
