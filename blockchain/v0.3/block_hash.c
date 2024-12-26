#include "blockchain.h"


int cpy_tx(transaction_t *tx, int idx, uint8_t *buf);
/**
 * block_hash- func
 * @block: block_t const *
 * @hash_buf: uint8_t []
 * Return: uint8_t
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int b_length = 56, tx_size = 0, t_length = 0;
	int8_t *buf, *current_pos;

	if (!block || !hash_buf)
		return (NULL);
	memset(hash_buf, 0, SHA256_DIGEST_LENGTH);
	b_length += block->data.len;
	tx_size = llist_size(block->transactions);
	if (tx_size > 0)
		t_length = b_length + tx_size * 32;
	buf = malloc(t_length);
	if (!buf)
		return (NULL);
	memcpy(buf, block, b_length);
	if (tx_size > 0)
	{
		current_pos += b_length;
		llist_for_each(block->transactions, cpy_tx, &current_pos);
	}
	sha256((int8_t *)block, t_length, hash_buf);
	free(buf);
	return (hash_buf);
}


/**
 * cpy_tx -		function to copy transaction id into buf
 * @tx:			transaction to copy id from
 * @idx:		index of the transaction,
 *				used to calculate the buf pos where the id must be copied
 * @buf:		buffer where to cpoy the id to
 * Return:		an integer
 */
int cpy_tx(transaction_t *tx, int idx, uint8_t *buf)
{
	int pos = 0;

	pos = 32 * idx;
	memcpy(buf + pos, tx, 32);
	return (0);
}

