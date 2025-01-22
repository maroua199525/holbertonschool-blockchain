#include "transaction.h"


/**
 * update_unspent -		updates the list of all unspent transaction outputs,
 *						given a list of processed transactions
 *
 * @transactions:		is the list of validated transactions
 *
 * @block_hash:			Hash of the validated Block that contains the
 *						transaction list transactions
 *
 * @all_unspent:		is the current list of unspent transaction outputs
 *
 * Return:				a new list of unspent transaction outputs
 *
 * All transaction inputs from each transaction in transactions should not
 * be included in the returned list of unspent transaction outputs
 *
 * All transaction outputs from each transaction in transactions should be
 * appended in the returned list of unspent transaction outputs
 *
 * The list all_unspent must be deleted upon success
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i, j, tx_size, out_size;
	tx_out_t *out = NULL;
	transaction_t *tx = NULL;
	unspent_tx_out_t *node = NULL;
	llist_t *new_unspent = NULL;

	new_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!new_unspent)
		return (NULL);
	filter_unspent(all_unspent, new_unspent, transactions);
	tx_size = llist_size(transactions);
	for (i = 0; i < tx_size; i++)
	{
		tx = llist_get_node_at(transactions, i);
		out_size = llist_size(tx->outputs);
		for (j = 0; j < out_size; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			node = unspent_tx_out_create(block_hash, tx->id, out);
			if (!node)
				return (NULL);
			llist_add_node(new_unspent, (llist_node_t)node, ADD_NODE_REAR);
		}
	}
	return (new_unspent);
}


/**
 * purge_unspent -	Deletes used unspent transactions.
 * @uto:			Unspent node.
 * @in_tx:			Verified transaction.
 * Return:			int
 */
int purge_unspent(unspent_tx_out_t *uto, tx_in_t *in_tx)
{
	if (!memcmp(uto, in_tx, 2 * SHA256_DIGEST_LENGTH) &&
		!memcmp(uto->out.hash, in_tx->tx_out_hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}


/**
 * filter_unspent -	Manages the process of updating the unspent transactions.
 * @all_unspent:	List to be filtered.
 * @new_unspent:	New list of unspent.
 * @transactions:	List of transactions.
 * Return:			Void.
 */
void filter_unspent(llist_t *all_unspent, llist_t *new_unspent,
	llist_t *transactions)
{
	int  i, j, tx_size, in_size, uto_size;
	tx_in_t *in = NULL;
	transaction_t *tx = NULL;
	unspent_tx_out_t *node = NULL;

	tx_size = llist_size(transactions);
	for (i = 0; i < tx_size; i++)
	{
		tx = llist_get_node_at(transactions, i);
		in_size = llist_size(tx->inputs);
		for (j = 0; j < in_size; j++)
		{
			in = llist_get_node_at(tx->inputs, j);
			llist_remove_node(all_unspent, (node_ident_t)purge_unspent,
				in, 1, NULL);
		}
	}
	uto_size = llist_size(all_unspent);
	for (i = 0; i < uto_size; i++)
	{
		node = llist_get_node_at(all_unspent, i);
		llist_add_node(new_unspent, (llist_node_t)node, ADD_NODE_REAR);
	}
}
