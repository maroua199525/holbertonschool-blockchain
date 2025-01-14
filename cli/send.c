#include "cli.h"


/**
 * send -			send an amount of coins
 *
 * @sender:			senders key
 *
 * @arg:			arguments
 *
 * @all_unspent:	list of unspent transactions
 *
 * @local_pool:		local pool of transactions
 *
 * Return:			transaction on SUCCESS, NULL otherwise
 */
transaction_t *send(EC_KEY *sender, char **arg, llist_t *all_unspent,
	llist_t *local_pool)
{
	transaction_t *tx = NULL;
	EC_KEY *receiver = NULL;
	uint32_t amount = 0;

	if (!arg[0] || !arg[1])
		return (send_error(0));
	if (!sender)
		return (send_error(1));
	if (!all_unspent)
		return (send_error(2));
	receiver = ec_from_pub((uint8_t const *)arg[1]);
	if (!receiver || check_num(arg[0]))
		return (send_error(3));
	amount = atoi(arg[0]);
	tx = transaction_create(sender, receiver, amount, all_unspent);
	if (!tx)
		return (send_error(4));
	if (!transaction_is_valid(tx, all_unspent))
		return (send_error(5));
	llist_add_node(local_pool, (llist_node_t)tx, ADD_NODE_REAR);
	return (tx);
}


/**
 * check_num -	checks if a string is a valid unsigned intger
 *
 * @str:		string to converto to integer
 *
 * Return:		0 if valid, non zero otherwise
 */
int check_num(char *str)
{
	int i = 0;

	while (str[i])
		if ((str[i] > '9') || (str[i] < '0'))
			return (-1);
	if (i > 10)
		return (-1);
	return (0);
}


/**
 * send_error -		manages send function error cases
 *
 * @error:			integer that indicates the error
 *
 * Return:			NULL
 */
void *send_error(int error)
{
	char *error_msg[] = {
		"No ammount or address specified.",
		"No sender specified.\n",
		"No unspent transaction available.",
		"Amount no valid or wrong address.",
		"Couldn't create transaction.",
		"Transaction wasn't valid."
	};

	printf("%s\n", error_msg[error]);
	return (NULL);
}
