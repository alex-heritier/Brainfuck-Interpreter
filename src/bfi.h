
#ifndef BFI_H
#define BFI_H

static const int MEMORY_SIZE = 30000;

struct bfi_state {
	unsigned char memory[MEMORY_SIZE];
	unsigned char *pointer;
	const char *instructions;
	char *ip;
	unsigned int instruction_size;
};

struct bfi_state *	create_bfi(const char *instructions, unsigned int length);
void				destroy_bfi(struct bfi_state *bfi);
int					run(struct bfi_state *bfi);

//debug
void				debug(struct bfi_state *bfi);

#endif