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
	int length = 56;

	if (!block || !hash_buf)
		return (NULL);
	length += block->data.len;
	sha256((int8_t *)block, length, hash_buf);
	return (hash_buf);
}
