#include "transaction.h"


/**
 * coinbase_create -	creates a coinbase transaction
 *
 * @receiver:			contains the public key of the miner, who will receive
 *						the coinbase coins
 *
 * @block_index:		is the index of the Block the coinbase transaction
 *						will belong to
 *
 * Return:				pointer to the created transaction upon success,
 *						or NULL upon failure
 *
 * The resulting transaction must contain a single transaction input and a
 *		single transaction output
 *
 * The transaction input should be zeroed, and not signed. To ensure this
 *		transaction input is unique, the block index will be copied in the
 *		first 4 bytes of the input’s tx_out_hash
 *
 * The amount in the transaction output should be exactly COINBASE_AMOUNT
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction = NULL;
	tx_in_t *in = NULL;
	uint8_t pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	transaction = malloc(sizeof(transaction_t));
	if (!transaction)
		return (NULL);
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
		return (cbc_failure(transaction));
	in = malloc(sizeof(tx_in_t));
	if (!in)
		return (cbc_failure(transaction));
	memset(in, 0, sizeof(tx_in_t));
	*((int *)(in->tx_out_hash)) = block_index;
	llist_add_node(transaction->inputs, (llist_node_t)in, ADD_NODE_REAR);
	ec_to_pub(receiver, pub);
	llist_add_node(transaction->outputs,
		(llist_node_t)tx_out_create(COINBASE_AMOUNT, pub), ADD_NODE_REAR);
	transaction_hash(transaction, transaction->id);
	return (transaction);
}


/**
 * cbc_failure -	exit function in case of failure
 *
 * @transaction:	transaction to be freed
 *
 * Return:			void pointer
 */
void *cbc_failure(transaction_t *transaction)
{
	if (transaction->inputs)
		llist_destroy(transaction->inputs, 1, NULL);
	if (transaction->outputs)
		llist_destroy(transaction->outputs, 1, NULL);
	free(transaction);
	return (NULL);
}
