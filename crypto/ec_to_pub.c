#include "hblk_crypto.h"


/**
 * ec_to_pub- func
 * @key: EC_KEY const *
 * @pub: uint8_t *
 * Return: uint8_t *
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_GROUP *g = NULL;
	const EC_POINT *p = NULL;
	point_conversion_form_t f;

	if (!key || !pub)
		return (NULL);
	g = EC_KEY_get0_group(key);
	if (!g)
		return (NULL);
	p = EC_KEY_get0_public_key(key);
	if (!p)
		return (NULL);
	f = EC_KEY_get_conv_form(key);
	EC_POINT_point2oct(g, p, f, (unsigned char *)pub, EC_PUB_LEN, NULL);
	return (pub);
}
