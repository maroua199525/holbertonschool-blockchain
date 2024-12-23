#include "blockchain.h"


/**
 * block_create- func
 * @prev: block_t const *
 * @data: int8_t const
 * @data_len: uint32_t
 * Return: block_t *
 */

block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *new_block = malloc(sizeof(block_t));

	if (!new_block)
		return (NULL);

	/* Initialize struct members to zero */
	memset(new_block, 0, sizeof(block_t));

	/* Block info initialisation*/
	if (prev)
	{
		new_block->info.index = prev->info.index + 1;
		memcpy(new_block->info.prev_hash, prev->hash,
			   sizeof(new_block->info.prev_hash));
	}
	new_block->info.timestamp = time(NULL);
	/* new_block->info.timestamp = 1537207477; */ /* hard code for tests */

	/* Block data initialisation */
	if (data && data_len)
	{
		if (data_len > BLOCKCHAIN_DATA_MAX)
			data_len = BLOCKCHAIN_DATA_MAX;
		memcpy(new_block->data.buffer, data, data_len);
		new_block->data.len = data_len;
	}

	/* transaction empty list creation */
	new_block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!new_block->transactions)
	{
		free(new_block);
		return (NULL);
	}

	return (new_block);
}