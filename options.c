#include <getopt.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"

void show_help();

struct tm_options parse_options(int argc, char **argv)
{
	static struct option options[] = {
		{ "width", required_argument, NULL, 'w' },
		{ "height", required_argument, NULL, 'h' },
		{ "mine-density",  required_argument, NULL, 'm'},
		{ "help", no_argument, NULL, 1 },
		{ NULL, 0, NULL, 0 }
	};

	struct tm_options tm_options = {20, 10, 0.1};

	char param;
	while ((param = getopt_long(argc, argv, "w:h:m:", options, NULL)) != -1) {
		switch (param) {
		case 'w': {
			uintmax_t value = strtoumax(optarg, NULL, 10);
			if (value != UINTMAX_MAX) {
				tm_options.width = (int)value;
			}
			break;
		}

		case 'h': {
			uintmax_t value = strtoumax(optarg, NULL, 10);
			if (value != UINTMAX_MAX) {
				tm_options.height = (int)value;
			}
			break;
		}

		case 'm': {
			float value = strtof(optarg, NULL);
			if (value != 0) {
				tm_options.mine_density = value;
			}
			break;
		}

		case 1: {
			show_help();
			exit(0);
		}
		}
	}
	
	return tm_options;
}

void show_help()
{
    pid_t pid = fork();
    if (pid == 0) { /* Child process */
        static char *argv[] = {"man", "terminal-mines", NULL};
        execv("/usr/bin/man", argv);
        exit(127); /* only if execv fails */
    } else { /* pid != 0; parent process */
        waitpid(pid, 0, 0); /* wait for child to exit */
    }
}
