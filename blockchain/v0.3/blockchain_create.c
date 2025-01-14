#include "blockchain.h"


/**
 * blockchain_create -	creates a blockchain and the genesis block
 *
 * Return:				the created blockchain with the genesis block in it
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;

	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(0);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || !blockchain->unspent)
	{
		blockchain_destroy(blockchain);
		return (NULL);
	}
	block = malloc(sizeof(block_t));
	if (!block)
	{
		blockchain_destroy(blockchain);
		return (NULL);
	}
	memset(block, 0, sizeof(block_t));
	memcpy(block, &_genesis, sizeof(block_t));
	llist_add_node(blockchain->chain, block, ADD_NODE_FRONT);
	return (blockchain);
}
