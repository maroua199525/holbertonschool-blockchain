#ifndef TRANSACTION_H
#define TRANSACTION_H



/* Coinbase amount */
# define COINBASE_AMOUNT	50


/* Libraries */
	#include <fcntl.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <unistd.h>

	#include <llist.h>
	#include "../../../crypto/hblk_crypto.h"



/* Provided structures related to transactions */

/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 *
 * @pub:    Receiver's public address
 *
 * @hash:   Hash of @amount and @pub. Serves as output ID
 */
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t;


/**
 * struct tx_in_s - Transaction input
 *
 * Description: A transaction input always refers to a previous
 * transaction output. The only exception is for a Coinbase transaction, that
 * adds new coins to ciruclation.
 *
 * @block_hash:  Hash of the Block containing the transaction @tx_id
 *
 * @tx_id:       ID of the transaction containing @tx_out_hash
 *
 * @tx_out_hash: Hash of the referenced transaction output
 *
 * @sig:         Signature. Prevents anyone from altering the content of the
 *               transaction. The transaction input is signed by the receiver
 *               of the referenced transaction output, using their private key
 */
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t;


/**
 * struct unspent_tx_out_s - Unspent transaction output
 *
 * Description: This structure helps identify transaction outputs that were not
 * used in any transaction input yet, making them "available".
 *
 * @block_hash: Hash of the Block containing the transaction @tx_id
 *
 * @tx_id:      ID of the transaction containing @out
 *
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t;


/**
 * struct transaction_s - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 *           Prevents further alteration of the transaction.
 *
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 *
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;



/* Auxiliar structures */

/**
 * struct tx_valid_s - Structure to ease transaction validation
 *
 * @all_unspent:	llist of unspent transactions output
 *
 * @flag:			flag to check if signature is correct
 *
 * @sum:			sum of all amounts related to the inputs
 *
 * @tx_id:			transaction id
 */
 typedef struct tx_valid_s
 {
	llist_t		*all_unspent;
	uint8_t		*flag;
	uint8_t		*sum;
	uint8_t		const *tx_id;
	
 } tv_t;


/**
 * struct inputs_search_s - Structure to ease the inputs search
 *
 * @inputs:		list of inputs
 *
 * @pub:		public key
 *
 * @sum:		sum of coins available
 */
typedef struct inputs_search_s
{
	llist_t		*inputs;
	uint8_t		*pub;
	uint32_t	*sum;
} is_t;


/**
 * struct in_signer_s - Structure to ease the inputs signature
 *
 * @tx_id:			pointer to the transaction id
 *
 * @key:			pointer to key used to sign
 *
 * @all_unspent:	llist of unspent transactions output
 */
typedef struct in_signer_s
{
	uint8_t		*tx_id;
	EC_KEY		*key;
	llist_t		*all_unspent;
} isg_t;



/* Functions prototypes */

/* v0.3 */

/* tx_out_create.c */
	tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

/* unspent_tx_out_create.c */
	unspent_tx_out_t *unspent_tx_out_create(
		uint8_t block_hash[SHA256_DIGEST_LENGTH],
		uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);

/* tx_in_create.c */
	tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);

/* transaction_hash.c */
	uint8_t *transaction_hash(transaction_t const *transaction,
		uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
	int cpy_in(tx_in_t *node, int idx, uint8_t *buffer);
	int cpy_out(tx_out_t *node, int idx, uint8_t *buffer);

/* tx_in_sign.c */
	sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		EC_KEY const *sender, llist_t *all_unspent);
	int find_block(unspent_tx_out_t *node, tx_in_t *in);
	int check_pub(EC_KEY const *key, uint8_t *pub);
	int sign_inputs(tx_in_t *in, int idx, isg_t *aux);

/* transaction_create.c */
	transaction_t *transaction_create(EC_KEY const *sender,
		EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);
	int search_inputs(unspent_tx_out_t *uto, int idx, is_t *node);
	void get_pub_key(EC_KEY const *key, uint8_t *pub);
	int check_owner(unspent_tx_out_t *node, uint8_t pub[EC_PUB_LEN]);
	void *transaction_failure(transaction_t *transaction);

/* transaction_is_valid.c */
	int transaction_is_valid(transaction_t const *transaction,
		llist_t *all_unspent);
	int tx_validator(tx_in_t *in, int idx, tv_t *aux);
	int output_coins_counter(tx_out_t *out, int idx, uint8_t *sumout);

/* coinbase_create.c */
	transaction_t *coinbase_create(EC_KEY const *receiver,
		uint32_t block_index);
	void *cbc_failure(transaction_t *transaction);

/* coinbase_is_valid.c */
	int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);

/* transaction_destroy.c */
	void transaction_destroy(transaction_t *transaction);

/* update_unspent.c */
	llist_t *update_unspent(llist_t *transactions,
		uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);
	int purge_unspent(unspent_tx_out_t *uto, tx_in_t *in_tx);
	void filter_unspent(llist_t *all_unspent, llist_t *new_unspent,
		llist_t *transactions);


#endif
