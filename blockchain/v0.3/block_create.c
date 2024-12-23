#include "blockchain.h"


/**
 * block_create- func
 * @prev: block_t const *
 * @data: int8_t const
 * @data_len: uint32_t
 * Return: block_t *
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t *nb = NULL;
	nb = malloc(sizeof(block_t));
	if (!nb)
		return (NULL);

	
	/* Initialize struct members to zero */
	memset(nb, 0, sizeof(block_t));

	if (prev)
	{
		nb->info.index = prev->info.index + 1;
		memcpy(nb->info.prev_hash, prev->hash,
			   sizeof(nb->info.prev_hash));
	}
	nb->info.timestamp = time(NULL);
	/* Block data initialisation */
	if (data && data_len)
	{
		if (data_len > BLOCKCHAIN_DATA_MAX)
			data_len = BLOCKCHAIN_DATA_MAX;
		memcpy(nb->data.buffer, data, data_len);
		nb->data.len = data_len;
	}

	nb->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!nb->transactions)
	{
		free(nb);
		return (NULL);
	}
	return (nb);
}
