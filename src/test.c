
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
		if (strcmp(argv[1], "-file") == 0) {
			char *file_contents;
			long input_file_size;
			FILE *input_file = fopen(argv[2], "rb");
			fseek(input_file, 0, SEEK_END);
			input_file_size = ftell(input_file);
			rewind(input_file);
			file_contents = malloc((input_file_size + 1) * (sizeof(char)));
			fread(file_contents, sizeof(char), input_file_size, input_file);
			fclose(input_file);
			file_contents[input_file_size] = 0;
			return file_contents;
		} else if (strcmp(argv[1], "-direct") == 0) {
			return argv[2];
		} else {
			return 0;
		}
	} else if (argc == 1) {
		return 0;
	} else {
		return 0;
	}
}

int main(int argc, char **argv)
{
	char *instructions = process_arguments(argc, argv);
	if (!instructions) {
		puts("error: usage: bfi <-file, -direct> <filename, code>");
		return 1;
	}
	struct bfi_state *bfi = create_bfi(instructions, strlen(instructions));
	int result = run(bfi);
	
	printf("\nFinished with error code %d.\n", result);
	if (argc == 3 && strcmp(argv[1], "-file") == 0) free(instructions);
	return 0;
}