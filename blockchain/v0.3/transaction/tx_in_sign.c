#include "transaction.h"


/**
 * tx_in_sign -		signs a transaction input, given the transaction id it is
 *					from
 *
 * @in:				points to the transaction input structure to sign
 *
 * @tx_id:			contains the ID (hash) of the transaction the transaction
 *					input to sign is stored in
 *
 * @sender:			contains the private key of the receiver of the coins
 *					contained in the transaction output referenced by the
 *					transaction input
 *
 * @all_unspent:	is the list of all unspent transaction outputs to date

 * Your function must verify the public key derived from the private key in
 * sender matches the public key stored in the transaction output referenced
 * by the transaction input to be signed
 *
 * Your function must sign the ID of the transaction tx_id the transaction
 * input is stored in, using sender‘s private key
 *
 * Your function must return a pointer to the resulting signature structure
 * upon success, or NULL upon failure
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
 * find_block -		compares the node data to the in data
 *
 * @node:			node to check the data
 *
 * @in:				data to look for
 *
 * Return:			an integer
 */
int find_block(unspent_tx_out_t *node, tx_in_t *in)
{
	if (!node || !in)
		return (0);
	if (memcmp(node->block_hash, in->block_hash, SHA256_DIGEST_LENGTH))
		return (0);
	if (memcmp(node->tx_id, in->tx_id, SHA256_DIGEST_LENGTH))
		return (0);
	if (memcmp(node->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}


/**
 * check_pub -		checks if the public key is correct
 *
 * @key:			key that contains the correct public key
 *
 * @pub:			public key to check
 *
 * Return:			an integer
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
 *
 * @in:				list of inputs to be signed
 *
 * @idx:			index of the node in the list, it's not used
 *
 * @aux:			auxiliar structure that contains all the parameters
 *					needed to sign
 *
 * Return:			an integer
 */
int sign_inputs(tx_in_t *in, int idx, isg_t *aux)
{
	(void)idx;
	tx_in_sign(in, aux->tx_id, aux->key, aux->all_unspent);
	return (0);
}

