#include "mem.h"
extern BLOCK_HEADER* first_header;

void coalesce(BLOCK_HEADER *header);

// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size) {
	// nothing to add in memory
	if (size <= 0)
		return NULL;

	// per block data
	int block_size, payload, size_t = size;
	int isFree = 0;		// if free block exists or not; 1: true
	BLOCK_HEADER *current = first_header;
	void *user_pointer;

	if ((8 + size) % 16 != 0)
		size_t = size + 16 - (8 + size) % 16;	// divisible by 16

	while (1) {
		// last block
		if (current->size_alloc == 1) {
			// return NULL if we didn't find a block
			if (!isFree)
				return NULL;

			// block_size = 8;
			break;
		}

		// per block data
		block_size = current->size_alloc & 0xFFFFFFFE;
		user_pointer = (void *)((unsigned long)current + 8);
		payload = current->payload;

		// free block
		if (!(current->size_alloc & 1)) {
			// find a free block that's big enough
			if (payload >= size) {
				isFree = 1;	// free block detected

				// allocate the block
				current->size_alloc = 8 + 1 + size_t;
				current->payload = size;

				// split if necessary
				BLOCK_HEADER *header = (BLOCK_HEADER *)((unsigned long)current + current->size_alloc - 1);

				// block not specified
				if (!((block_size - (current->size_alloc - 1) - 8) < 16)) {
					header->size_alloc = block_size - (current->size_alloc - 1);
					header->payload = block_size - (current->size_alloc - 1) - 8;
				}

				return user_pointer;
			}
			else
				return NULL;
		}
		current = (BLOCK_HEADER *)((unsigned long)current + block_size);
	}

	return NULL;
}

// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr) {
	// traverse the list and check all pointers to find the correct block 
	// if you reach the end of the list without finding it return -1
	BLOCK_HEADER *current;
	unsigned long add_header = (unsigned long)ptr - sizeof(BLOCK_HEADER);
	current = (BLOCK_HEADER*)add_header;

	if (ptr == NULL || !(current->size_alloc & 1))
		return -1;

	// free the block 
	current->size_alloc = current->size_alloc & 0xFFFFFFFE;
	current->payload = current->size_alloc - 8;

	// coalesce adjacent free blocks
	coalesce(current);

	return 0;
}

void coalesce(BLOCK_HEADER *current) {
	int block_size, t_block_size = 0;

	// after
	BLOCK_HEADER *header = (BLOCK_HEADER *)((unsigned long)current + current->size_alloc);
	if (!(header->size_alloc & 1)) {
		// if next block is free, then coalesce
		current->size_alloc += header->size_alloc;
		current->payload += header->size_alloc;
	}

	// before
	if (current == first_header)
		return;

	header = first_header;	// initiate to first header
	while (1) {
		block_size = header->size_alloc & 0xFFFFFFFE;
		t_block_size += block_size;

		// last block
		if (header->size_alloc == 1)
			break;

		// free block
		if (!(header->size_alloc & 1)) {
			// find previous block
			if ((BLOCK_HEADER *)((unsigned long)current - t_block_size) == first_header) {
				header->size_alloc += current->size_alloc;
				header->payload = header->size_alloc - 8;

				break;
			}
		}

		header = (BLOCK_HEADER *)((unsigned long)header + block_size);
	}
}