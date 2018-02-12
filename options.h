#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <stdint.h>

struct tm_options {
	int width;
	int height;
	float mine_density;
	bool adventure_mode;
};

struct tm_options parse_options(int argc, char **argv);

#endif
