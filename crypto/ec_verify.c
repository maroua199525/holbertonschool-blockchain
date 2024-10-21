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
	int len = (int)sig->len;
	EC_KEY *k = NULL;

	if (!key || !msg || !sig)
		return (0);
	k = (EC_KEY *)key;
	if (ECDSA_verify(0, msg, msglen, (unsigned char *)sig->sig, len, k) != 1)
		return (0);
	return (1);
}