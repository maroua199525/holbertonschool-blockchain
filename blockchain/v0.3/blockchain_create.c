#include "blockchain.h"


/**
 * blockchain_create- func
 * Return: blockchain_t *
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;

	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || !blockchain->unspent)
	{
		free(blockchain);
		return (NULL);
	}
	block = malloc(sizeof(block_t));
	if (!block)
	{
		free(blockchain->chain), free(blockchain);
		return (NULL);
	}
	*block = _genesis;
	if (llist_add_node(blockchain->chain, block, ADD_NODE_FRONT) == -1)
	{
		free(blockchain->chain), free(blockchain), free(block);
		return (NULL);
	}
	return (blockchain);
}
