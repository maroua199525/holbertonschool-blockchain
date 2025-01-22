#include "../blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);


/**
 * main -	func
 * Return:	int
 */
int main(void)
{
	int i = 0, size;
	blockchain_t *bc = NULL;
	unspent_tx_out_t *uto = NULL;

	bc = blockchain_deserialize("bc_save.sv");

	_blockchain_print(bc);
	printf("\nunspent:\n");
	size = llist_size(bc->unspent);
	for (i = 0; i < size; i++)
	{
		uto = llist_get_node_at(bc->unspent, i);
		_print_hex_buffer(uto->block_hash, SHA256_DIGEST_LENGTH);
		printf("\n");
	}

	blockchain_destroy(bc);
	return (0);
}
