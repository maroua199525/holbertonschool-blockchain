#include "../blockchain.h"


/**
 * transaction_hash- func
 * @transaction: transaction_t const *
 * @hash_buf: uint8_t *
 * Return: uint8_t *
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
 * cpy_in- func
 * @node: tx_in_t *
 * @idx: int
 * @buffer: uint8_t *
 * Return: int
 */
int cpy_in(tx_in_t *node, int idx, uint8_t *buffer)
{
	int pos = 32 * 3 * idx;

	memcpy(buffer + pos, node, 96);
	return (0);
}


/**
 * cpy_out- func
 * @node: tx_out_t *
 * @idx: int
 * @buffer: uint8_t *
 * Return: int
 */
int cpy_out(tx_out_t *node, int idx, uint8_t *buffer)
{
	int pos = 32 * idx;

	memcpy(buffer + pos, node->hash, 32);
	return (0);
}
