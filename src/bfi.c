
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
	int return_code = 0;
	if (bfi->pointer >= (MEMORY_SIZE + bfi->memory)) {	//if pointer overflows
		fprintf(stderr, "error: overflow %ld\n", bfi->pointer - bfi->memory);
		return_code = 1;
	}
	bfi->pointer++;
	return return_code;
}

static int decrement_pointer(struct bfi_state *bfi)
{
	int return_code = 0;
	if (bfi->pointer < bfi->memory) {	//if pointer underflows
		fprintf(stderr, "error: underflow %ld\n", bfi->pointer - bfi->memory);
		return_code = 2;
	}
	bfi->pointer--;
	return return_code;
}

static int increment_byte(struct bfi_state *bfi)
{
	(*bfi->pointer)++;	//increment memory
	
	return 0;
}

static int decrement_byte(struct bfi_state *bfi)
{
	(*bfi->pointer)--;	//decrement memory
	
	return 0;
}

static int output(const struct bfi_state *bfi)
{
	putchar(*bfi->pointer);
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
	if (*bfi->pointer == 0) {	//if mp does not point at a 0
		bfi->ip++;
		int ignore_count = 0;
		while (!(*bfi->ip == ']' && ignore_count == 0)) {
			//while current instruction isnt a ']' or ignore count isnt 0
			if (*bfi->ip == '[') {
				ignore_count++;
			} else if (*bfi->ip == ']') {
				ignore_count--;
			}
			bfi->ip++;
			if (bfi->ip >= bfi->instructions + bfi->instruction_size) {
				return 5;
			}
		}
	}
	return 0;
}

static int backward(struct bfi_state *bfi)
{
	if (*bfi->pointer != 0) {
		bfi->ip--;
		int ignore_count = 0;
		while (!(*bfi->ip == '[' && ignore_count == 0)) {
			if (*bfi->ip == ']') {
				ignore_count++;
			} else if (*bfi->ip == '[') {
				ignore_count--;
			}
			bfi->ip--;
			if (bfi->ip < bfi->instructions) {
				return 6;
			}
		}
		bfi->ip--;
	}
	return 0;
}

static int hashtag(const struct bfi_state *bfi)	//lol
{
	for (unsigned char *i = (unsigned char *)bfi->memory; i < bfi->memory + 10; ++i) {
		putchar(*i);
	}
	
	return 0;
}

//interface

struct bfi_state *create_bfi(const char *instructions, unsigned int size)
{
	struct bfi_state *bfi = malloc(sizeof(struct bfi_state));
	bfi->pointer = bfi->memory;
	bfi->instructions = instructions;
	bfi->ip = (char *)instructions;
	bfi->instruction_size = size;
	memset(bfi->memory, 0, MEMORY_SIZE * sizeof(bfi->memory[0]));
	
	return bfi;
}

void destroy_bfi(struct bfi_state *bfi)
{
	free(bfi);
}

static int execute(struct bfi_state *bfi, const char instruction)
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
		//debug(bfi);
		int result = execute(bfi, *bfi->ip);
		if (result)
			return result;
		bfi->ip++;
	}
	return 0;
}

//debug

void debug(struct bfi_state *bfi)
{
	printf("*ip: %c\tip: %ld\tmp: %ld\t*mp: %d\n",
		*bfi->ip,
		bfi->ip - bfi->instructions,
		bfi->pointer - bfi->memory,
		*bfi->pointer);
}

