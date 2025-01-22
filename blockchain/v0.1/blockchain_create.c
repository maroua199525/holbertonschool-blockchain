#include "blockchain.h"


/**
 * blockchain_create- func
 * Return: blockchain_t *
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;

	blockchain = malloc(sizeof(blockchain));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(0);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	block = malloc(sizeof(block_t));
	if (!block)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}
	block->info.index = 0;
	block->info.difficulty = 0;
	block->info.timestamp = 1537578000;
	block->info.nonce = 0;
	memset(block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memset(block->data.buffer, 0, BLOCKCHAIN_DATA_MAX);
	strcpy((char *)&block->data.buffer, "Holberton School");
	block->data.len = strlen((char *)block->data.buffer);
	memcpy((char *)block->hash,
			"\xc5\x2c\x26\xc8\xb5\x46\x16\x39"
			"\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
			"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1"
			"\xd3\xd7\xff\x2f\x04\x51\x58\x03",
			SHA256_DIGEST_LENGTH);
		/* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
	llist_add_node(blockchain->chain, block, ADD_NODE_FRONT);
	return (blockchain);
}
