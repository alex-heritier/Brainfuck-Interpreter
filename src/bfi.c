
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfi.h"

/*
> 	Increment the pointer.
< 	Decrement the pointer.
+ 	Increment the byte at the pointer.
- 	Decrement the byte at the pointer.
. 	Output the byte at the pointer.
, 	Input a byte and store it in the byte at the pointer.
[ 	Jump forward past the matching ] if the byte at the pointer is zero.
] 	Jump backward to the matching [ unless the byte at the pointer is zero.
*/

//commands

static int increment_pointer(struct bfi_state *bfi)
{
	bfi->pointer++;
	return 0;
}

static int decrement_pointer(struct bfi_state *bfi)
{
	bfi->pointer--;
	return 0;
}

static int increment_byte(struct bfi_state *bfi)
{
	if (bfi->pointer >= MEMORY_SIZE + bfi->memory) puts("FUCK!");
	(*bfi->pointer)++;
	
	return bfi->pointer >= MEMORY_SIZE + bfi->memory;
}

static int decrement_byte(struct bfi_state *bfi)
{
	if (bfi->pointer < 0) puts("FUCK!");
	(*bfi->pointer)--;
	
	return bfi->pointer < 0;
}

static int output(struct bfi_state *bfi)
{
	printf("%c", *bfi->pointer);
	return 0;
}

static int input(struct bfi_state *bfi)
{
	*bfi->pointer = getchar();
	char c = 0;
	while ((c = getchar()) != '\n' && c != EOF);	//flush stdin
	
	return 0;
}

static int forward(struct bfi_state *bfi)
{
	if (!*bfi->pointer) {
		while (*bfi->ip != ']') {
			bfi->ip++;
			if (bfi->ip >= bfi->instructions + bfi->instruction_size) {
				return 1;
			}
		}
		bfi->ip++;
	}
	return 0;
}

static int backward(struct bfi_state *bfi)
{
	if (*bfi->pointer) {
		while (*bfi->ip != '[') {
			bfi->ip--;
			if (bfi->ip < bfi->instructions) {
				return 1;
			}
		}
	}
	return 0;
}

static int hashtag(struct bfi_state *bfi)
{
	int offset = bfi->instruction_size > 10 ? 10 : bfi->instruction_size;
	for (char *i = bfi->instructions; i < bfi->instructions + offset; ++i) {
		putchar(*i);
	}
	putchar('\n');
	
	return 0;
}

//interface

struct bfi_state *create_bfi(char *instructions, unsigned int size)
{
	struct bfi_state *bfi = malloc(sizeof(struct bfi_state));
	bfi->pointer = bfi->memory;
	bfi->instructions = instructions;
	bfi->ip = instructions;
	bfi->instruction_size = size;
	memset(bfi->memory, 0, MEMORY_SIZE * sizeof(bfi->memory[0]));
	
	return bfi;
}

static int execute(struct bfi_state *bfi, char instruction)
{
	switch (instruction) {
		case '>':
			return increment_pointer(bfi);
		case '<':
			return decrement_pointer(bfi);
		case '+':
			return increment_byte(bfi);
		case '-':
			return decrement_byte(bfi);
		case '.':
			return output(bfi);
		case ',':
			return input(bfi);
		case '[':
			return forward(bfi);
		case ']':
			return backward(bfi);
		case '#':
			return hashtag(bfi);
		default:
			return 0;
	}
}

int run(struct bfi_state *bfi)
{
	while (bfi->ip != bfi->instructions + bfi->instruction_size) {
		int result = execute(bfi, *bfi->ip);
		//printf("instruction: %c\tresult: %d\n", *bfi->ip, result);
		if (result)
			return 1;
		bfi->ip++;
	}
	return 0;
}

//debug

void print(struct bfi_state *bfi)
{
	for (unsigned char *i = bfi->pointer; i < bfi->memory + MEMORY_SIZE; i++) {
		putc(*i, stdout);
	}
	putchar('\n');
}

