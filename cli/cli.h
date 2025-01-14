#ifndef CLI_H
#define CLI_H


/* Libraries */

	#include "string.h"
	#include "stdio.h"
	#include "stdlib.h"

	#include "unistd.h"


	#include "../crypto/hblk_crypto.h"
	#include "../blockchain/v0.3/blockchain.h"


/* Variables */

	extern char const *cmd_name[];


/* Functions */

	/* cli.c */
		int cmd_maker(char **cmd);
		void cmd_clean(char **cmd);
		void cli_usage(void);
		int select_cmd(char *command);

	/* wallet.c */
		EC_KEY *wallet_load(char *path);
		void wallet_save(EC_KEY *key, char *path);

	/* send.c */
		transaction_t *send(EC_KEY *sender, char **arg, llist_t *all_unspent,
			llist_t *local_pool);
		int check_num(char *str);
		void *send_error(int error);

	/* mine.c */
		block_t *mine(llist_t *local_pool, blockchain_t *blockchain,
			EC_KEY *receiver);
		int search_tx(transaction_t *tx, transaction_t *aux);
		void purge_tx(llist_t *local_pool, llist_t *unspent);

	/* info.c */
		void info(blockchain_t *blockchain, llist_t *local_pool);



#endif
