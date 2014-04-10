
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "bfi.h"

/*
Hello world in Brainfuck
++++++++++[>+++++++>++++++++++>+++>+<<<<-]\
>++.>+.+++++++..+++.>++.<<+++++++++++++++.\
>.+++.------.--------.>+.>.
*/

char *process_arguments(int argc, char **argv)
{
	if (argc >= 3) {
		if (strcmp(argv[1], "-f") == 0) {
			//file
		} else if (strcmp(argv[1], "-d") == 0) {
			return argv[2];
		}
		return argv[2];
	} else if (argc == 1) {
		return "+++>++<[->[-]++<]";
	} else {
		return 0;
	}
}

int main(int argc, char **argv)
{
	char *instructions = process_arguments(argc, argv);
	if (!instructions) {
		puts("error: usage: bfi <switch> <argument>");
		return 1;
	}
	struct bfi_state *bfi = create_bfi(instructions, strlen(instructions));
	int result = run(bfi);
	
	printf("\nFinished with error code %d.\n", result);
	return 0;
}