#include "transaction.h"


/**
 * transaction_create -		creates a transaction
 *
 * @sender:			contains the private key of the transaction sender
 *
 * @receiver:		contains the public key of the transaction receiver
 *
 * @amount:			is the amount to send
 *
 * @all_unspent:	is the list of all the unspent outputs to date
 *
 * Return:			return a pointer to the created transaction upon success,
 *					or NULL upon failure
 *
 *
 * Select a set of unspent transaction outputs from all_unspent, which public
 *		keys match sender‘s private key
 *
 * Create transaction inputs from these selected unspent transaction outputs
 *
 * The function must fail if sender does not possess enough coins (total amount
 *		of selected unspent outputs lower than amount)
 *
 * Create a transaction output, sending amount coins to receiver’s public key
 *
 * If the total amount of the selected unspent outputs is greater than amount,
 *		create a second transaction output, sending the leftover back to sender
 *
 * Compute transaction ID (hash)
 *
 * Sign transaction inputs using the previously computed transaction ID
 *
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction = NULL;
	uint8_t spub[EC_PUB_LEN], rpub[EC_PUB_LEN];
	uint32_t sum = 0, dif = 0;
	is_t aux;
	isg_t bux;

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);
	transaction = malloc(sizeof(transaction_t));
	if (!transaction)
		return (NULL);
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
		return (transaction_failure(transaction));
	get_pub_key(sender, spub);
	aux.inputs = transaction->inputs;
	aux.pub = spub;
	aux.sum = &sum;
	llist_for_each(all_unspent, (node_func_t)search_inputs, &aux);
	if (sum < amount)
		return (transaction_failure(transaction));
	get_pub_key(receiver, rpub);
	llist_add_node(transaction->outputs,
		(llist_node_t)tx_out_create(amount, rpub), ADD_NODE_REAR);
	dif = sum - amount;
	if (dif)
		llist_add_node(transaction->outputs,
			(llist_node_t)tx_out_create(dif, spub), ADD_NODE_REAR);
	if (!transaction_hash(transaction, transaction->id))
		return (transaction_failure(transaction));
	bux.tx_id = transaction->id;
	bux.key = (EC_KEY *)sender;
	bux.all_unspent = all_unspent;
	llist_for_each(transaction->inputs, (node_func_t)sign_inputs, &bux);
	return (transaction);
}


/**
 * search_inputs -	searchs for the matching unspent transaction
 *
 * @uto:			unspent node to check
 *
 * @idx:			index of the node, not used
 *
 * @node:			auxiliar structure
 *
 * Return:			an integer
 */
int search_inputs(unspent_tx_out_t *uto, int idx, is_t *node)
{
	(void)idx;
	if (check_owner(uto, node->pub))
	{
		llist_add_node(node->inputs, (llist_node_t)tx_in_create(uto),
			ADD_NODE_REAR);
		*(node->sum) += uto->out.amount;
	}
	return (0);
}


/**
 * get_pub_key -	gets public key from key
 *
 * @key:			key that contains the public key
 *
 * @pub:			buffer to store the public key
 *
 * Return:			void
 */
void get_pub_key(EC_KEY const *key, uint8_t *pub)
{
	EC_POINT const *ec_pub = NULL;
	EC_GROUP const *ec_group = NULL;

	ec_pub = EC_KEY_get0_public_key(key);
	ec_group = EC_KEY_get0_group(key);
	EC_POINT_point2oct(ec_group, ec_pub, POINT_CONVERSION_UNCOMPRESSED,
		pub, EC_PUB_LEN, NULL);
}


/**
 * check_owner -	check if public keys match
 *
 * @node:			unspent node to check
 *
 * @pub:			buffer with the public key
 *
 * Return:			an integer
 */
int check_owner(unspent_tx_out_t *node, uint8_t pub[EC_PUB_LEN])
{
	return (memcmp(node->out.pub, pub, EC_PUB_LEN) ? 0 : 1);
}


/**
 * transaction_failure -	manages failure
 *
 * @transaction:			transaction node to free
 *
 * Return:					NULL
 */
void *transaction_failure(transaction_t *transaction)
{
	if (transaction->inputs)
		llist_destroy(transaction->inputs, 1, NULL);
	if (transaction->outputs)
		llist_destroy(transaction->outputs, 1, NULL);
	free(transaction);
	return (NULL);
}
