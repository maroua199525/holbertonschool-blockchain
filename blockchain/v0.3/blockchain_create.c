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


/**
 * rebuild_tx -		Rebuilds a transactions list from a file.
 * @fd:				Stream to read from.
 * Return:			Transaction.
 */
transaction_t *rebuild_tx(FILE *fd)
{
	uint32_t i = 0, in_size = 0, out_size = 0;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;
	transaction_t *tx = NULL;

	tx = malloc(sizeof(transaction_t));
	if (!tx)
		return (NULL);
	fread(tx->id, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fd);
	fread(&in_size, sizeof(uint32_t), 1, fd);
	fread(&out_size, sizeof(uint32_t), 1, fd);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < in_size; i++)
	{
		in = rebuild_tx_in(fd);
		if (!in)
		{
			transaction_destroy(tx);
			return (NULL);
		}
		llist_add_node(tx->inputs, (llist_node_t)in, ADD_NODE_REAR);
	}
	for (i = 0; i < out_size; i++)
	{
		out = rebuild_tx_out(fd);
		if (!out)
		{
			transaction_destroy(tx);
			return (NULL);
		}
		llist_add_node(tx->outputs, (llist_node_t)out, ADD_NODE_REAR);
	}
	return (tx);
}


/**
 * rebuild_tx_in -	Rebuild in transaction.
 * @fd:				Stream to read from.
 * Return:			In transaction.
 */
tx_in_t *rebuild_tx_in(FILE *fd)
{
	tx_in_t *in = NULL;

	in = malloc(sizeof(tx_in_t));
	if (!in)
		return (NULL);
	fread(in, sizeof(tx_in_t), 1, fd);
	return (in);
}


/**
 * rebuild_tx_out -	Rebuild out transaction.
 * @fd:				Stream to read from.
 * Return:			Out transaction.
 */
tx_out_t *rebuild_tx_out(FILE *fd)
{
	tx_out_t *out = NULL;

	out = malloc(sizeof(tx_out_t));
	if (!out)
		return (NULL);
	fread(out, sizeof(tx_out_t), 1, fd);
	return (out);
}
