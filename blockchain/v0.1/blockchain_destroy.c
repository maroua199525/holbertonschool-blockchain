#include "blockchain.h"


/**
 * blockchain_destroy- func
 * @blockchain: blockchain_t *
 * Return: void
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	free(blockchain);
}
