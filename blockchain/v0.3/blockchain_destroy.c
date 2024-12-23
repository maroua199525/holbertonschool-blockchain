#include "blockchain.h"


/**
 * blockchain_destroy- func
 * @blockchain: blockchain_t *
 * Return: void
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (blockchain->chain)
		llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	if (blockchain->unspent)
		llist_destroy(blockchain->unspent, 1, NULL);
	free(blockchain);
}
