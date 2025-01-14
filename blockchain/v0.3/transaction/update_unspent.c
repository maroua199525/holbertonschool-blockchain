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
	int i = 0, size = 0, j = 0, pos = 0;
	tx_out_t *out = NULL;
	transaction_t *tx = NULL;
	unspent_tx_out_t *node = NULL;
	llist_t *new_unspent = NULL;

	new_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!new_unspent)
		return (NULL);
	size = llist_size(transactions);
	for (; i < size; i++)
	{
		tx = llist_get_node_at(transactions, i);
		pos = llist_size(tx->outputs);
		for (j = 0; j < pos; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			node = unspent_tx_out_create(block_hash, tx->id, out);
			if (!node)
			{
				llist_destroy(new_unspent, 1, NULL);
				return (NULL);
			}
			llist_add_node(new_unspent, (llist_node_t)node, ADD_NODE_REAR);
		}
	}
	llist_destroy(all_unspent, 0, NULL);
	return (new_unspent);
}