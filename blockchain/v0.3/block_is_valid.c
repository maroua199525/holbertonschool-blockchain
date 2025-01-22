#include "blockchain.h"


/**
 * block_is_valid -		check that each Block’s hash is valid
 *
 * @block:				block to check if hash is valid
 *
 * @prev_block:			previous block to check referenced hashes
 *
 * @all_unspent:		transactions list to check
 *
 * Return:				EXIT_SUCCESS or EXIT_FAILURE
 *
 * A block must contain at least one transaction (at least the coinbase
 * transaction)
 *
 * The first transaction in a Block must be a valid coinbase transaction
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	int size = 0, i = 0;
	transaction_t const *tx = NULL;

	if (!block)
		return (EXIT_FAILURE);
	if ((!block->info.index && prev_block) ||
			(block->info.index && !prev_block))
		return (EXIT_FAILURE);
	if (!block->info.index && compare_to_genesis(block))
		return (EXIT_FAILURE);
	if (block->info.index && check_block(block, prev_block))
		return (EXIT_FAILURE);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (EXIT_FAILURE);
	size = llist_size(block->transactions);
	if (!size)
		return (EXIT_FAILURE);
	for (; i < size; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!i)
		{
			if (!coinbase_is_valid(tx, block->info.index))
				return (EXIT_FAILURE);
		}
		else if (!transaction_is_valid(tx, all_unspent))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


/**
 * compare_to_genesis- func
 * @block: block_t const *
 * Return: int
 */
int compare_to_genesis(block_t const *block)
{
	int i = 0;

	if (block->info.timestamp != 1537578000)
		return (EXIT_FAILURE);
	if (block->info.nonce != 0)
		return (EXIT_FAILURE);
	for (; i < SHA256_DIGEST_LENGTH; i++)
		if (block->info.prev_hash[i] != 0)
			return (EXIT_FAILURE);
	if (memcmp(block->data.buffer, "Holberton School", 16))
		return (EXIT_FAILURE);
	if (block->data.len != 16)
		return (EXIT_FAILURE);
	if (memcmp(block->hash,
		"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
		"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03",
		SHA256_DIGEST_LENGTH))
		return (EXIT_FAILURE);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


/**
 * check_block- func
 * @block: bock_t const *
 * @prev_block: block_t const *
 * Return: int
 */
int check_block(block_t const *block, block_t const *prev_block)
{
	uint8_t digest[SHA256_DIGEST_LENGTH];

	if (block->info.index != (prev_block->info.index + 1))
		return (EXIT_FAILURE);
	if (!prev_block->info.index)
	{
		if (memcmp(prev_block, &_genesis, sizeof(_genesis)))
			return (EXIT_FAILURE);
	}
	else
	{
		if (!block_hash(prev_block, digest))
			return (EXIT_FAILURE);
		if (memcmp(prev_block->hash, digest, SHA256_DIGEST_LENGTH))
			return (EXIT_FAILURE);
	}
	if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (EXIT_FAILURE);
	if (!block_hash(block, digest))
		return (EXIT_FAILURE);
	if (memcmp(block->hash, digest, SHA256_DIGEST_LENGTH))
		return (EXIT_FAILURE);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
