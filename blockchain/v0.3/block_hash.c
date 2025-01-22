#include "blockchain.h"


/**
 * block_hash -		computes the blocks hash
 *
 * @block:			block to be hashed
 *
 * @hash_buf:		buffer to store the hash
 *
 * Return:			pointer to the buffer on success or NULL on failure
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int b_length = 0, tx_size = 0, t_length = 0;
	uint8_t *buf = NULL;

	if (!block || !hash_buf)
		return (NULL);
	b_length += sizeof(block_info_t) + block->data.len;
	tx_size = llist_size(block->transactions);
	t_length = b_length + tx_size * SHA256_DIGEST_LENGTH;
	buf = malloc(t_length);
	if (!buf)
		return (NULL);
	memcpy(buf, block, b_length);
	llist_for_each(block->transactions, (node_func_t)cpy_tx, buf + b_length);
	sha256((int8_t *)buf, t_length, hash_buf);
	free(buf);
	return (hash_buf);
}


/**
 * cpy_tx -		function to copy transaction id into buf
 *
 * @tx:			transaction to copy id from
 *
 * @idx:		index of the transaction,
 *				used to calculate the buf pos where the id must be copied
 *
 * @buf:		buffer where to cpoy the id to
 *
 * Return:		an integer
 */
int cpy_tx(transaction_t *tx, int idx, uint8_t *buf)
{
	int pos = 0;

	pos = SHA256_DIGEST_LENGTH * idx;
	memcpy(buf + pos, tx, SHA256_DIGEST_LENGTH);
	return (0);
}

