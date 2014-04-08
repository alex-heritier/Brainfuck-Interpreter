
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "bfi.h"

int main(int argc, char **argv)
{
	char *instructions;
	if (argc > 1) {
		instructions = argv[1];
	} else {
		instructions = "+++++[>+++++ +++++<-]>--[+.]";
	}
	struct bfi_state *bfi = create_bfi(instructions, strlen(instructions));
	
	run(bfi);
	
	putchar('\n');
	return 0;
}