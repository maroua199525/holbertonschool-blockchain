#include "hblk_crypto.h"

/**
 * ec_from_pub - converts a public key to an EC key.
 * @pub: contains the public key to be converted
 *
 * Return: return a pointer to the created EC_KEY structure
 * upon success,or NULL upon failure
 **/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key;
	EC_POINT *point;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	point = EC_POINT_new(EC_KEY_get0_group(key));
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	if (!EC_POINT_oct2point(EC_KEY_get0_group(key), point, pub, EC_PUB_LEN, NULL)
		|| !EC_KEY_set_public_key(key, point))
	{
		EC_KEY_free(key);
		EC_POINT_free(point);
		return (NULL);
	}

	EC_POINT_free(point);
	return (key);
}
