#include "hblk_crypto.h"


/**
 * ec_verify- func
 * @key: EC_KEY const *
 * @msg: uint8_t const *
 * @msglen: size_t
 * @sig: sig_t const *
 * Return: int
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);
	if (ECDSA_verify(0, msg, msglen, (unsigned char *)sig->sig,
			sig->len, (EC_KEY *)key) != 1)
		return (0);
	return (1);
}
