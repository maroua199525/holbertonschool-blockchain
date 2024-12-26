#include "blockchain.h"


/**
 * block_hash- func
 * @block: block_t const *
 * @hash_buf: uint8_t []
 * Return: uint8_t
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int b_length, tx_size = 0;
	int8_t *buf, *current_pos;

	if (!block || !hash_buf)
		return (NULL);
	memset(hash_buf, 0, SHA256_DIGEST_LENGTH);
	b_length = sizeof(block->info) + block->data.len;
	tx_size = llist_size(block->transactions);
	b_length += tx_size * SHA256_DIGEST_LENGTH;
	buf = malloc(b_length);
	if (!buf)
		return (NULL);
	current_pos = buf;
	memcpy(current_pos, block, sizeof(block->info) + block->data.len);
	current_pos += sizeof(block->info) + block->data.len;
	llist_for_each(block->transactions, (node_func_t)cpy_tx, current_pos);
	sha256((int8_t *)buf, b_length, hash_buf);
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
