#ifndef BLOCKCHAIN_V0_2
#define BLOCKCHAIN_V0_2


#define BLOCKCHAIN_DATA_MAX 1024

/* Difficulty to mine */
#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5

/* Coinbase amount */
# define COINBASE_AMOUNT    50


/* Libraries */
	#include <fcntl.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <unistd.h>

	#include <llist.h>
	#include "provided/endianness.h"
	#include "../../crypto/hblk_crypto.h"



/**
 * struct tx_valid_s - Structure to ease transaction validation
 *
 * @all_unspent:	llist of unspent transactions output
 * @flag:			flag to check if signature is correct
 * @sum:			sum of all amounts related to the inputs
 * @tx_id:			transaction id
 */
 typedef struct tx_valid_s
 {
	llist_t		*all_unspent;
	uint8_t		*flag;
	uint8_t		*sum;
	const uint8_t     *tx_id;
	
 } tv_t;


/**
 * struct inputs_search_s - Structure to ease the inputs search
 *
 * @inputs:		list of inputs
 * @pub:		public key
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
 * @tx_id: 		pointer to the transaction id
 * @key: 		pointer to key used to sign
 * @all_unspent:	llist of unspent transactions output
 */
typedef struct in_signer_s
{
	uint8_t		*tx_id;
	EC_KEY		*key;
	llist_t		*all_unspent;
} isg_t;


/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	/*
	 * The order of the elements in this structure should remain the same.
	 * It was designed so every element of this structure is aligned and
	 * doesn't require padding from the compiler.
	 * Therefore, it is possible to use the structure as an array of char,
	 * on any architecture.
	 */
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;


/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
	/*
	 * @buffer must stay first, so we can directly use the structure as
	 * an array of char
	 */
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;


/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
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
 * @tx_id:       ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
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
 * @tx_id:      ID of the transaction containing @out
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
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;



/**
 * struct block_s - Block structure
 *
 * @info:         Block info
 * @data:         Block data
 * @transactions: List of transactions
 * @hash:         256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info; /* This must stay first */
	block_data_t    data; /* This must stay second */
	llist_t     *transactions;
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;


/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain:   Linked list of Blocks
 * @unspent: Linked list of unspent transaction outputs
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;



/* Functions */

/* v0.1 */
	blockchain_t *blockchain_create(void);
	block_t *block_create(block_t const *prev, int8_t const *data,
		uint32_t data_len);
	void block_destroy(block_t *block);
	void blockchain_destroy(blockchain_t *blockchain);
	uint8_t *block_hash(block_t const *block,
		uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
	int blockchain_serialize(blockchain_t const *blockchain, char const *path);
	int save_block(block_t *block, uint32_t index, FILE *fd);
	blockchain_t *blockchain_deserialize(char const *path);
	block_t *rebuild_block(FILE *fd);
	int block_is_valid(block_t const *block, block_t const *prev_block);
	int compare_to_genesis(block_t const *block);
	int check_block(block_t const *block, block_t const *prev_block);

/* v0.2 */
	int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
		uint32_t difficulty);
	int get_leading_zeroes(uint8_t const hash[SHA256_DIGEST_LENGTH]);
	void block_mine(block_t *block);
	uint32_t blockchain_difficulty(blockchain_t const *blockchain);

/* v0.3 */
	tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
	unspent_tx_out_t *unspent_tx_out_create(
		uint8_t block_hash[SHA256_DIGEST_LENGTH],
		uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);
	tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);
	uint8_t *transaction_hash(transaction_t const *transaction,
		uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
	int cpy_in(tx_in_t *node, int idx, uint8_t *buffer);
	int cpy_out(tx_out_t *node, int idx, uint8_t *buffer);
	sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		EC_KEY const *sender, llist_t *all_unspent);
	int find_block(unspent_tx_out_t *node, tx_in_t *in);
	int check_pub(EC_KEY const *key, uint8_t *pub);
	transaction_t *transaction_create(EC_KEY const *sender,
		EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);
	int search_inputs(unspent_tx_out_t *uto, int idx, is_t *node);
	void get_pub_key(EC_KEY const *key, uint8_t *pub);
	int check_owner(unspent_tx_out_t *node, uint8_t pub[EC_PUB_LEN]);
	void *transaction_failure(transaction_t *transaction);
	int sign_inputs(tx_in_t *in, int idx, isg_t *aux);
	int transaction_is_valid(transaction_t const *transaction,
		llist_t *all_unspent);
	int tx_validator(tx_in_t *in, int idx, tv_t *aux);
	int output_coins_counter(tx_out_t *out, int idx, uint8_t *sumout);
	transaction_t *coinbase_create(EC_KEY const *receiver,
		uint32_t block_index);
	void *cbc_failure(transaction_t *transaction);
	int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);
	void transaction_destroy(transaction_t *transaction);
	int cpy_tx(transaction_t *tx, int idx, uint8_t *buf);


#endif
