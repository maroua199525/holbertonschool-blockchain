#include "../blockchain.h"


/**
 * transaction_is_valid -	checks whether a transaction is valid
 *
 * @transaction:			points to the transaction to verify
 *
 * @all_unspent:			is the list of all unspent transaction outputs to
 *							date
 *
 * Return:					1 if the transaction is valid, and 0 otherwise
 *
 * The computed hash of the transaction must match the hash stored in it
 *
 * Each input in the transaction must refer to an unspent output in all_unspent
 *
 * Each input’s signature must be verified using the public key stored in the
 *		referenced unspent output
 *
 * The total amount of inputs must match the total amount of outputs
 */
int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t ha[SHA256_DIGEST_LENGTH];
	uint8_t flag = 1, sumin = 0, sumout = 0;
	tv_t aux;

	transaction_hash(transaction, ha);
	if (memcmp(transaction->id, ha, SHA256_DIGEST_LENGTH))
		return (0);
	aux.all_unspent = all_unspent;
	aux.flag = &flag;
	aux.sum = &sumin;
	aux.tx_id = transaction->id;
	llist_for_each(transaction->inputs, (node_func_t)tx_validator, &aux);
	if (!flag)
		return (0);
	llist_for_each(transaction->outputs, (node_func_t)output_coins_counter,
		&sumout);
	if (sumin != sumout)
		return (0);
	return (1);
}


/**
 * tx_validator -	validates an input node
 *
 * @in:				node to check if is valid
 *
 * @idx:			node's index, it's not used
 *
 * @aux:			auxiliar strucutre with data to validate node
 *
 * Return:			an integer
 */
int tx_validator(tx_in_t *in, int idx, tv_t *aux)
{
	unspent_tx_out_t *uto = NULL;
	EC_KEY *key = NULL;

	(void)idx;
	uto = llist_find_node(aux->all_unspent, (node_ident_t)find_block, in);
	if (!uto)
	{
		*(aux->flag) = 0;
		return (0);
	}
	*(aux->sum) += uto->out.amount;
	key = ec_from_pub(uto->out.pub);
	if (!ec_verify(key, aux->tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		*(aux->flag) = 0;
	return (0);
}


/**
 * output_coins_counter -	counts the amount of coins in the outputs list
 *
 * @out:					node to chekc for the amount
 *
 * @idx:					index of the node, not used
 *
 * @sumout:					pointer to the counter
 *
 * Return:					an integer
 */
int output_coins_counter(tx_out_t *out, int idx, uint8_t *sumout)
{
	(void)idx;
	*(sumout) += out->amount;
	return (0);
}
