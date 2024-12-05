#include "blockchain.h"


/**
 * block_mine- func
 * @block: block_t *
 * Return: void
 */
void block_mine(block_t *block)
{
    if (!block)  /* Return early if block is NULL */
        return;

    /* Use a do-while loop to ensure the hash is computed at least once */
    do {
        block->info.nonce += 1;  /* Increment nonce */
        block_hash(block, block->hash);  /* Calculate the hash */
    } while (!hash_matches_difficulty(block->hash, block->info.difficulty));  /* Check if hash matches difficulty */
}