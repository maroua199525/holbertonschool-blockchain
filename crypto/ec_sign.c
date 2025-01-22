#include "hblk_crypto.h"


/**
 * ec_sign- func
 * @key: EC_KEY const *
 * @msg: uint8_t const *
 * @msglen: size_t
 * @sig: sig_t *
 * Return: uint8_t *
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t *sig)
{
	unsigned int len = EC_PUB_LEN;

	if (!key || !msg || !sig)
		return (NULL);
	if (!ECDSA_sign(0, msg, msglen, sig->sig, &len, (EC_KEY *)key))
		return (NULL);
	sig->len = len;
	return (sig->sig);
}
