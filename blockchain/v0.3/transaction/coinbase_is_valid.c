#include "transaction.h"


/**
 * coinbase_is_valid -		checks whether a coinbase transaction is valid
 *
 * @coinbase:				points to the coinbase transaction to verify
 *
 * @block_index:			is the index of the Block the coinbase transaction
 *							will belong to
 *
 * Return:					1 if the coinbase transaction is valid,
 *							and 0 otherwise
 *
 *
 * The coinbase transaction must verify the following:
 *
 *		The computed hash of the transaction must match the hash stored in it
 *
 *		The transaction must contain exactly 1 input
 *
 *		The transaction must contain exactly 1 output
 *
 *		The transaction input’s tx_out_hash first 4 bytes must match the
 *		block_index
 *
 *		The transaction input’s block_hash, tx_id, and signature must be zeroed
 *
 *		The transaction output amount must be exactly COINBASE_AMOUNT
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;
	uint8_t hash[SHA256_DIGEST_LENGTH], arr[SHA256_DIGEST_LENGTH];

	if (!coinbase)
		return (0);
	transaction_hash(coinbase, hash);
	if (memcmp(hash, coinbase->id, SHA256_DIGEST_LENGTH))
		return (0);
	if (llist_size(coinbase->inputs) != 1)
		return (0);
	if (llist_size(coinbase->outputs) != 1)
		return (0);
	in = llist_get_head(coinbase->inputs);
	memset(arr, 0, SHA256_DIGEST_LENGTH);
	if (memcmp(in->block_hash, &arr, SHA256_DIGEST_LENGTH))
		return (0);
	if (memcmp(in->tx_id, &arr, SHA256_DIGEST_LENGTH))
		return (0);
	*(int *)arr = block_index;
	if (memcmp(in->tx_out_hash, &arr, SHA256_DIGEST_LENGTH))
		return (0);
	out = llist_get_head(coinbase->outputs);
	if (out->amount != COINBASE_AMOUNT)
		return (0);
	return (1);
}
