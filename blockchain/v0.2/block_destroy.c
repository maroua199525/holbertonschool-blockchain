#include "blockchain.h"


/**
 * block_destroy- func
 * @block: block_t *
 * Return: void
 */
void block_destroy(block_t *block)
{
	free(block);
}
