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
		return (ec_to_pub_errors(2));
	p = EC_KEY_get0_public_key(key);
	if (!p)
		return (ec_to_pub_errors(3));
	f = EC_KEY_get_conv_form(key);
	EC_POINT_point2oct(g, p, f, (unsigned char *)pub, EC_PUB_LEN, NULL);
	return (pub);
}


/**
 * ec_to_pub_errors- func
 * @error: int
 * Return: void *
 */
void *ec_to_pub_errors(int error)
{
	if (error == 1)
		printf("No key passed.\n");
	if (error == 2)
		printf("Couldn't get group.\n");
	if (error == 3)
		printf("Couldn't get point.\n");
	return (NULL);
}
