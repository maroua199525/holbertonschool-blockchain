#include "cli.h"


/**
 * mine -		mines a new block
 *
 * @local_pool:	list of transactions
 *
 * @blockchain:	blockchain to add the node to
 *
 * @receiver:	receivers key
 *
 * Return:		the block created
 */
block_t *mine(llist_t *local_pool, blockchain_t *blockchain, EC_KEY *receiver)
{
	block_t *block = NULL, *prev = NULL;
	int i = 0, size = 0;
	transaction_t *tx = NULL, *coinbase = NULL;
	llist_t *aux = NULL;

	if (!blockchain || !blockchain->chain || !blockchain->unspent)
	{
		printf("No valid blockchain.\n");
		return (NULL);
	}
	prev = llist_get_tail(blockchain->chain);
	block = block_create(prev, (int8_t *)"New Block", 8);
	block->info.difficulty = blockchain_difficulty(blockchain);
	coinbase = coinbase_create(receiver, block->info.index);
	llist_add_node(block->transactions, (llist_node_t)coinbase, ADD_NODE_FRONT);
	size = llist_size(local_pool);
	purge_tx(local_pool, blockchain->unspent);
	for (; i < size; i++)
	{
		tx = llist_get_node_at(local_pool, i);
		if (transaction_is_valid(tx, blockchain->unspent))
			llist_add_node(block->transactions, (llist_node_t)tx,
			ADD_NODE_REAR);
	}
	block_mine(block);
	if (!block_is_valid(block, prev, blockchain->unspent))
	{
		printf("Couldn't create block.\n");
		block_destroy(block);
		return (NULL);
	}
	llist_add_node(blockchain->chain, (llist_node_t)block, ADD_NODE_REAR);
	aux = update_unspent(block->transactions, block->hash, blockchain->unspent);
	if (!aux)
		printf("Couldn't update unspent.\n");
	return (block);
}


/**
 * search_tx -	checks wether the current node is the one looked for
 *
 * @tx:			transaction node of the list
 *
 * @aux:		transaction node as reference
 *
 * Return:		an integer
 */
int search_tx(transaction_t *tx, transaction_t *aux)
{
	return (!memcmp(tx, aux, SHA256_DIGEST_LENGTH));
}


/**
 * purge_tx -	deletes invalid transactions from local pool
 *
 * @local_pool:	list of transactions
 *
 * @unspent:	list of unspent transactions
 *
 * Return:		void
 */
void purge_tx(llist_t *local_pool, llist_t *unspent)
{
	int i = 0, size = 0;
	transaction_t *tx = NULL;

	size = llist_size(local_pool);
	while (i < size)
	{
		tx = llist_get_node_at(local_pool, i);
		if (!transaction_is_valid(tx, unspent))
		{
			llist_remove_node(local_pool, (node_ident_t)search_tx, tx, 1,
				(node_dtor_t)transaction_destroy);
			i--;
			size--;
		}
		i++;
	}
}
