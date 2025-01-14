#include "transaction.h"


/**
 * transaction_hash -	computes the ID (hash) of a transaction
 *
 * @transaction:		points to the transaction to compute the hash of
 *
 * @hash_buf:			is a buffer in which to store the computed hash
 *
 * Return:				pointer to hash_buf
 *
 *
 * transaction->id must be left unchanged
 *
 * The transaction ID must be a hash of a buffer containing the following
 * information:
 *		Each input’s block_hash (32 bytes), tx_id (32 bytes) and tx_out_hash
 *		(32 bytes), followed by
 *
 *		Each output’s hash (32 bytes)
 *
 *		For example, the buffer for a transaction with 4 inputs and 2 outputs
 *		would be 448 bytes long (32 * 3 * 4 + 32 * 2)
 */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int in_size = 0, out_size = 0, size = 0, pos = 0;
	uint8_t *buffer = NULL;

	if (!transaction || !hash_buf)
		return (NULL);
	in_size = llist_size(transaction->inputs);
	pos = 32 * 3 * in_size;
	out_size = llist_size(transaction->outputs);
	size = pos +  32 * out_size;
	buffer = malloc(size);
	if (!buffer)
		return (NULL);
	llist_for_each(transaction->inputs, (node_func_t)cpy_in, buffer);
	llist_for_each(transaction->outputs, (node_func_t)cpy_out, buffer + pos);
	sha256((int8_t *)buffer, size, hash_buf);
	free(buffer);
	return (hash_buf);
}


/**
 * cpy_in -		copies tx_in_t into buffer
 *
 * @node:		node in the list
 *
 * @idx:		index of the node, not used
 *
 * @buffer:		buffer to copy data in
 *
 * Return:		an integer
 */
int cpy_in(tx_in_t *node, int idx, uint8_t *buffer)
{
	int pos = 32 * 3 * idx;

	memcpy(buffer + pos, node, 96);
	return (0);
}


/**
 * cpy_out -	copies tx_out_t into buffer
 *
 * @node:		node in the list
 *
 * @idx:		index of the node, not used
 *
 * @buffer:		buffer to copy data in
 *
 * Return:		an integer
 */
int cpy_out(tx_out_t *node, int idx, uint8_t *buffer)
{
	int pos = 32 * idx;

	memcpy(buffer + pos, node->hash, 32);
	return (0);
}
