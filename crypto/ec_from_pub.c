#include "hblk_crypto.h"


/**
 * ec_from_pub- func
 * @pub: uint8_t const *
 * Return: EC_KEY *
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;

	if (!pub)
		return (NULL);
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);
	if (!EC_KEY_oct2key(key, (unsigned char *)pub, EC_PUB_LEN, NULL))
		return (NULL);
	EC_KEY_set_private_key(key, NULL);
	return (key);
}
