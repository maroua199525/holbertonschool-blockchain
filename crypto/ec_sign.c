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
	unsigned int len = SIG_MAX_LEN;
	EC_KEY *k = NULL;

	if (!key || !msg || !sig)
		return (NULL);
	k = (EC_KEY *)key;
	if (!ECDSA_sign(0, msg, msglen, sig->sig, &len, k))
		return (NULL);
	sig->len = len;
	return (sig->sig);
}