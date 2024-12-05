#include "blockchain.h"


/**
 * block_mine- func
 * @block: block_t *
 * Return: void
 */
void block_mine(block_t *block)
{
	if (block)
		while (!hash_matches_difficulty(block->hash, block->info.difficulty))
		{
			block_hash(block, block->hash);
			block->info.nonce++;
		}
}