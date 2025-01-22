#include "hblk_crypto.h"


/**
 * sha256- func
 * @s: int8_t const *
 * @len: size_t
 * @digest: uint8_t *
 * Return: uint8_t *
 */
uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);
	if (!SHA256((unsigned char *)s, len, digest))
		return (NULL);
	return (digest);
}
