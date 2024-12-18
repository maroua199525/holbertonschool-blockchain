#include "../blockchain.h"


/**
 * tx_out_create- func
 * @amount: uint32_t
 * @pub: uint8_t *
 * Return: tx_out_t *
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	int length = 0;
	tx_out_t *out_node = NULL;

	out_node = malloc(sizeof(tx_out_t));
	if (!out_node)
		return (NULL);
	out_node->amount = amount;
	memcpy(out_node->pub, pub, EC_PUB_LEN);
	length = sizeof(amount) + EC_PUB_LEN;
	sha256((int8_t *)out_node, length, out_node->hash);
	return (out_node);
}
