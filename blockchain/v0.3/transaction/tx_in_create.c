#include "transaction.h"


/**
 * tx_in_create -	allocates and initializes a transaction input structure
 *
 * @unspent:		points to the unspent transaction output to be converted
 *					to a transaction input
 *
 * Return:			pointer to the created transaction input upon success,
 *					or NULL upon failure
 *
 * The created transaction input’s signature structure must be zeroed
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *ti = NULL;

	if (!unspent)
		return (NULL);
	ti = malloc(sizeof(tx_in_t));
	if (!ti)
		return (NULL);
	memcpy(ti->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(ti->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(ti->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	memset(ti->sig.sig, 0, SIG_MAX_LEN);
	ti->sig.len = 0;
	return (ti);
}
