
#ifndef BFI_H
#define BFI_H

static const int MEMORY_SIZE = 30000;

struct bfi_state {
	unsigned char memory[MEMORY_SIZE];
	unsigned char *pointer;
	char *instructions;
	char *ip;
	unsigned int instruction_size;
};

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

struct bfi_state *	create_bfi(char *instructions, unsigned int length);
int					run(struct bfi_state *bfi);

//debug
void				print(struct bfi_state *bfi);

#endif