#include "cli.h"



/**
 * info -		prints some blockchain data
 *
 * @blockchain:	referenced blockchain
 *
 * @local_pool:	transactions list
 *
 * Return:		void
 */
void info(blockchain_t *blockchain, llist_t *local_pool)
{
	int no_blocks = 0, no_unspent = 0, no_tx = 0;

	if (blockchain)
	{
		if (blockchain->chain)
			no_blocks = llist_size(blockchain->chain);
		if (blockchain->unspent)
			no_unspent = llist_size(blockchain->unspent);
	}
	if (local_pool)
		no_tx = llist_size(local_pool);
	printf("No. of blocks:\t\t\t\t%d\n", no_blocks);
	printf("No. of unspent transaction output:\t%d\n", no_unspent);
	printf("No. of pennding transactions:\t\t%d\n", no_tx);
}
