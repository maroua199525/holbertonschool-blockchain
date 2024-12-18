#include "../blockchain.h"


/**
 * unspent_tx_out_create- func
 * @block_hash: uint8_t *
 * @tx_id: uint8_t *
 * @out: tx_out_t const *
 * Return: unspent_tx_out_t *
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *uto = NULL;

	if (!block_hash || !tx_id || !out)
		return (NULL);
	uto = malloc(sizeof(unspent_tx_out_t));
	if (!uto)
		return (NULL);
	memcpy(uto->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(uto->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&uto->out, out, sizeof(tx_out_t));
	return (uto);
}
