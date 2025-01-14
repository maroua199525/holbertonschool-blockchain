#include "blockchain.h"


/**
 * block_mine- func
 * @block: block_t *
 * Return: void
 */
void block_mine(block_t *block)
{
	if (!block)
		return;
	block_hash(block, block->hash);
	while (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		block->info.nonce++;
		block_hash(block, block->hash);
	}
}
