#include "../blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);


block_t *add_node(int8_t *str, blockchain_t *bc);



/**
 * main -	main
 * Return:	int
 */
int main(void)
{
	int i, size;
	block_t *b = NULL;
	blockchain_t *bc = NULL;
	unspent_tx_out_t *uto = NULL;

	bc = blockchain_create();

	b = add_node((int8_t *)"Node 1", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);

	b = add_node((int8_t *)"Node 2", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);

	b = add_node((int8_t *)"Node 3", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);

	b = add_node((int8_t *)"Node 4", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);

	b = add_node((int8_t *)"Node 5", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);

	b = add_node((int8_t *)"Node 6", bc);
	bc->unspent = update_unspent(b->transactions, b->hash, bc->unspent);


	_blockchain_print(bc);
	printf("\nunspent:\n");
	size = llist_size(bc->unspent);
	for (i = 0; i < size; i++)
	{
		uto = llist_get_node_at(bc->unspent, i);
		_print_hex_buffer(uto->block_hash, SHA256_DIGEST_LENGTH);
		printf("\n");
	}

	blockchain_serialize(bc, "bc_save.sv");
	blockchain_destroy(bc);
	return (0);
}


/**
 * add_node -	adds a node
 * @str:		string
 * @bc:			blockchain
 * @Return:		void
 */
block_t *add_node(int8_t *str, blockchain_t *bc)
{
	uint32_t len = 0;
	block_t *b = NULL, *prev = NULL;
	transaction_t *tx = NULL;
	EC_KEY *key = NULL;

	while (str[len])
		len++;
	prev = llist_get_tail(bc->chain);
	b = block_create(prev, str, len);
	b->info.difficulty = blockchain_difficulty(bc);
	key = ec_create();
	tx = coinbase_create(key, b->info.index);
	llist_add_node(b->transactions, (llist_node_t)tx, ADD_NODE_FRONT);
	block_mine(b);
	llist_add_node(bc->chain, (llist_node_t)b, ADD_NODE_REAR);
	free(key);
	return (b);
}
