#include "../blockchain.h"


/**
 * tx_in_sign- func
 * @in: tx_in_t *
 * @tx_id: uint8_t const *
 * @sender: EC_KEY const *
 * @all_unspent: llist_t *
 * Return: sig_t *
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender, llist_t *all_unspent)
{
	unspent_tx_out_t *uto = NULL;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);
	uto = llist_find_node(all_unspent, (node_ident_t)find_block, in);
	if (check_pub(sender, uto->out.pub))
		return (NULL);
	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);
	return (&in->sig);
}


/**
 * find_block- func
 * @node: unspent_tx_out_t *
 * @in: tx_in_t *
 * Return: int
 */
int find_block(unspent_tx_out_t *node, tx_in_t *in)
{
	if (memcmp(node->block_hash, in->block_hash, SHA256_DIGEST_LENGTH))
		return (0);
	if (memcmp(node->tx_id, in->tx_id, SHA256_DIGEST_LENGTH))
		return (0);
	if (memcmp(node->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}


/**
 * check_pub- func
 * @key: EC_KEY const *
 * @pub: uint8_t *
 * Return: int
 */
int check_pub(EC_KEY const *key, uint8_t *pub)
{
	EC_POINT const *ec_pub = NULL;
	EC_GROUP const *ec_group = NULL;
	uint8_t oct_pub[EC_PUB_LEN], len = 0;

	ec_pub = EC_KEY_get0_public_key(key);
	ec_group = EC_KEY_get0_group(key);
	len = EC_POINT_point2oct(ec_group, ec_pub, POINT_CONVERSION_UNCOMPRESSED,
		oct_pub, EC_PUB_LEN, NULL);
	return (memcmp(pub, oct_pub, len));
}


/**
 * sign_inputs-		signs all the inputs
 * @in:				list of inputs to be signed
 * @idx:			index of the node in the list, it's not used
 * @aux:			auxiliar structure that contains all the parameters
 *					needed to sign
 * Return:			an integer
 */
int sign_inputs(tx_in_t *in, int idx, isg_t *aux)
{
	(void)idx;
	tx_in_sign(in, aux->tx_id, aux->key, aux->all_unspent);
	return (0);
}

