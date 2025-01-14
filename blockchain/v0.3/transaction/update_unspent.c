#include "transaction.h"


/**
 * unspent_match - Utility function to compare unspent_tx_out
 * from all unspent to the tx passed as inputs
 * @utxo: Pointer to unspent tx_out_t
 * @tx_in: Pointer to transaction input 'node'
 * Return: 1 for 'success' match, 0 otherwise
 */

int unspent_match(unspent_tx_out_t *utxo, tx_in_t *tx_in)
{
	if (memcmp(utxo->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH) ||
		memcmp(utxo->tx_id, tx_in->tx_id, SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}


/**
 * chkyfhy chkkkkk 
**/
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i = 0, size = 0, j = 0, pos = 0, k;
	tx_out_t *out = NULL;
	transaction_t *tx = NULL;
	unspent_tx_out_t *node = NULL;
	tx_in_t *in_node;
	llist_t *new_unspent = NULL;
	unspent_tx_out_t *utxo;


	new_unspent = llist_create(MT_SUPPORT_FALSE);
	if (!new_unspent)
		return (NULL);
	size = llist_size(transactions);
	for (i = 0; i < llist_size(all_unspent); i++)
	{
		utxo = llist_get_node_at(all_unspent, i);
		for (j = 0; j < size; j++)
		{
			tx = llist_get_node_at(transactions, j);
			for (k = 0; k < llist_size(tx->inputs); k++)
			{
				in_node = llist_get_node_at(tx->inputs, k);
				if (unspent_match(utxo, in_node))
					break;
			}
			if (k < llist_size(tx->inputs))
				break;
		}
		if (j == size)
			llist_add_node(new_unspent, utxo, ADD_NODE_REAR);
		else
			free(utxo);
	}
	for (i = 0; i < size; i++)
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
	llist_destroy(all_unspent, 1, NULL);
	return (new_unspent);
}
