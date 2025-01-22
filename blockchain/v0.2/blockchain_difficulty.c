#include "blockchain.h"


/**
 * blockchain_difficulty- func
 * @blockchain: blockchain_t const *
 * Return: uint32_t
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	int mod = 0, idx = 0, elapsed_time = 0, eet = 0, dif = 0;
	block_t *last = NULL, *prev = NULL;

	if (!blockchain || !blockchain->chain)
		return (0);
	last = llist_get_tail(blockchain->chain);
	mod = last->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL;
	if (!last->info.index || mod)
		return (last->info.difficulty);
	idx = llist_size(blockchain->chain) - DIFFICULTY_ADJUSTMENT_INTERVAL;
	prev = llist_get_node_at(blockchain->chain, idx);
	eet = BLOCK_GENERATION_INTERVAL * DIFFICULTY_ADJUSTMENT_INTERVAL;
	elapsed_time = last->info.timestamp - prev->info.timestamp;
	if (elapsed_time < (eet / 2))
		dif = last->info.difficulty + 1;
	if (elapsed_time > (eet * 2))
		dif = last->info.difficulty - 1;
	return (dif);
}
