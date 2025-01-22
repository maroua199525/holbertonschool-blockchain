#include "blockchain.h"


/**
 * blockchain_serialize- func
 * @blockchain: blockchain_t const *
 * @path: char const *
 * Return: int
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	uint32_t no_blocks = 0;
	FILE *fd = NULL;

	if (!blockchain || !path)
		return (-1);
	fd = fopen(path, "w");
	if (!fd)
		return (-1);
	if (fseek(fd, 0, SEEK_SET) == -1)
		return (-1);
	no_blocks = llist_size(blockchain->chain);
	fprintf(fd, "HBLK0.1%c", 1);
	fwrite(&no_blocks, sizeof(uint8_t), 4, fd);
	fflush(fd);
	if (llist_for_each(blockchain->chain, (node_func_t)save_block, fd) == -1)
		return (-1);
	fclose(fd);
	return (0);
}


/**
 * save_block- func
 * @block: block_t *
 * @index: uint32_t
 * @fd: FILE *
 * Return: int
 */
int save_block(block_t *block, uint32_t index, FILE *fd)
{
	(void)index;
	fwrite(block, sizeof(uint8_t), 56, fd);
	fwrite(&block->data.len, sizeof(uint8_t), 4, fd);
	fwrite(&block->data.buffer, sizeof(uint8_t), block->data.len, fd);
	fwrite(&block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fd);
	fflush(fd);
	return (0);
}
