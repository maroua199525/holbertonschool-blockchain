#include "blockchain.h"


/**
 * blockchain_deserialize- func
 * @path: char const *
 * Return: blockchain_t *
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	uint8_t buffer[8], header[8] = {'H', 'B', 'L', 'K', '0', '.', '1', 1};
	uint32_t no_blocks = 0, i = 0;
	FILE *fd = NULL;
	block_t *block = NULL;
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
	fread(&no_blocks, sizeof(uint32_t), 1, fd);
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(0);
	if (!blockchain->chain)
	{
		blockchain_destroy(blockchain);
		return (NULL);
	}
	for (; i < no_blocks; i++)
	{
		block = rebuild_block(fd);
		if (llist_add_node(blockchain->chain, block, ADD_NODE_REAR) == -1)
		{
			blockchain_destroy(blockchain);
			return (NULL);
		}
	}
	fclose(fd);
	return (blockchain);
}


/**
 * rebuild_block- func
 * @fd: FILE *
 * Return: block_t *
 */
block_t *rebuild_block(FILE *fd)
{
	block_t *block = NULL;

	block = malloc(sizeof(block_t));
	if (!block)
		return (NULL);
	fread(&block->info, sizeof(uint8_t), 56, fd);
	fread(&block->data.len, sizeof(uint32_t), 1, fd);
	fread(&block->data, sizeof(uint8_t), block->data.len, fd);
	fread(block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fd);
	return (block);
}
