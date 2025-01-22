#include "blockchain.h"


/**
 * blockchain_deserialize- func
 * @path: char const *
 * Return: blockchain_t *
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	uint8_t buffer[8], header[8] = "\x48\x42\x4c\x4b\x30\x2e\x33\x01";
	FILE *fd = NULL;
	blockchain_t *blockchain = NULL;

	if (!path)
		return (NULL);
	fd = fopen(path, "rb");
	if (!fd)
		return (NULL);
	fseek(fd, 0, SEEK_SET);
	fread(buffer, sizeof(uint8_t), 8, fd);
	if (memcmp(buffer, header, 8))
		return (NULL);
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		blockchain_destroy(blockchain);
		return (NULL);
	}
	if (!rebuild_lists(blockchain, fd))
		return (NULL);
	fclose(fd);
	return (blockchain);
}


/**
 * rebuild_lists -		rebuilds lists
 * @blockchain:			pointer to blockchain
 * @fd:					stream to read from
 * Return:				NULL on failure, pointer to the blockchain in success
 */
blockchain_t *rebuild_lists(blockchain_t *blockchain, FILE *fd)
{
	int32_t no_blocks = 0, no_unspent = 0, i = 0, j = 0;
	unspent_tx_out_t *uto = NULL;
	block_t *block = NULL;

	fread(&no_blocks, sizeof(uint32_t), 1, fd);
	fread(&no_unspent, sizeof(uint32_t), 1, fd);
	for (; i < no_blocks; i++)
	{
		block = rebuild_block(fd);
		if (llist_add_node(blockchain->chain, (llist_node_t)block,
			ADD_NODE_REAR) == -1)
		{
			blockchain_destroy(blockchain);
			return (NULL);
		}
	}
	if (no_unspent >= 0)
	{
		blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
		if (!blockchain->unspent)
		{
			blockchain_destroy(blockchain);
			return (NULL);
		}
		for (; j < no_unspent; j++)
		{
			uto = rebuild_uto(fd);
			if (llist_add_node(blockchain->unspent, (llist_node_t)uto,
				ADD_NODE_REAR) == -1)
			{
				blockchain_destroy(blockchain);
				return (NULL);
			}
		}
	}
	else
		blockchain->unspent = NULL;
	return (blockchain);
}


/**
 * rebuild_block- func
 * @fd: FILE *
 * Return: block_t *
 */
block_t *rebuild_block(FILE *fd)
{
	int32_t i = 0, size = 0;
	block_t *block = NULL;
	transaction_t *tx = NULL;

	block = malloc(sizeof(block_t));
	if (!block)
		return (NULL);
	memset(block, 0, sizeof(block_t));
	fread(&block->info, sizeof(block_info_t), 1, fd);
	fread(&block->data.len, sizeof(uint32_t), 1, fd);
	fread(block->data.buffer, sizeof(int8_t), block->data.len, fd);
	fread(block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fd);
	fread(&size, sizeof(uint32_t), 1, fd);
	if (size == -1)
		block->transactions = NULL;
	else
	{
		block->transactions = llist_create(MT_SUPPORT_FALSE);
		for (; i < size; i++)
		{
			tx = rebuild_tx(fd);
			if (!tx)
			{
				block_destroy(block);
				return (NULL);
			}
			llist_add_node(block->transactions, (llist_node_t)tx,
				ADD_NODE_REAR);
		}
	}
	return (block);
}


/**
 * rebuild_uto- func
 * @fd: FILE *
 * Return: unspent_tx_out_t *
 */
unspent_tx_out_t *rebuild_uto(FILE *fd)
{
	unspent_tx_out_t *uto = NULL;

	uto = malloc(sizeof(unspent_tx_out_t));
	if (!uto)
		return (NULL);
	memset(uto, 0, sizeof(unspent_tx_out_t));
	fread(uto, sizeof(unspent_tx_out_t), 1, fd);
	return (uto);
}


/**
 * save_unspent -	serializes an unspent transaction
 *
 * @uto:			transaction to serialize
 *
 * @index:			transactions index
 *
 * @fd:				file descriptor to write data
 *
 * Return:			an integer
 */
int save_unspent(unspent_tx_out_t *uto, uint32_t index, FILE *fd)
{
	(void)index;
	fwrite(uto, sizeof(unspent_tx_out_t), 1, fd);
	return (0);
}
