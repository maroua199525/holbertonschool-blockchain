#include "blockchain.h"


/**
 * block_create- func
 * @prev: block_t const *
 * @data: int8_t const
 * @data_len: uint32_t
 * Return: block_t *
 */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
    block_t *nb = malloc(sizeof(block_t));
    uint32_t dl = 0;

    if (!nb)
        return (NULL);

    /* Initialize all fields to 0 */
    memset(nb, 0, sizeof(block_t));

    /* Set block index and previous hash if prev is not NULL */
    if (prev)
    {
        nb->info.index = prev->info.index + 1;
        memcpy(nb->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
    }

    nb->info.timestamp = time(NULL);

    /* Limit data length and copy */
    if (data && data_len > 0)
    {
        dl = (data_len > BLOCKCHAIN_DATA_MAX) ? BLOCKCHAIN_DATA_MAX : data_len;
        memcpy(nb->data.buffer, data, dl);
        nb->data.len = dl;
    }

    /* Create an empty transaction list */
    nb->transactions = llist_create(MT_SUPPORT_FALSE);
    if (!nb->transactions)
    {
        free(nb);
        return (NULL);
    }

    /* Zero the hash field */
    memset(nb->hash, 0, SHA256_DIGEST_LENGTH);

    return (nb);
}
