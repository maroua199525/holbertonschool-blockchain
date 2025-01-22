#include "blockchain.h"


/**
 * hash_matches_difficulty- func
 * @hash: uint_t const *
 * @difficulty: uint32_t
 * Return: int
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
		uint32_t difficulty)
{
	uint32_t leading_zeroes	= 0;

	leading_zeroes = get_leading_zeroes(hash);
	return ((difficulty <= leading_zeroes) ? 1 : 0);
}


/**
 * get_leading_zeroes- func
 * @hash: uint8_t const *
 * Return: int
 */
int get_leading_zeroes(uint8_t const hash[SHA256_DIGEST_LENGTH])
{
	int i = 0, j = 0, leading_zeroes = 0;
	char byte = 1;

	for (; i < SHA256_DIGEST_LENGTH; i++)
	{
		byte <<= 7;
		for (j = 0; j < 8; j++)
		{
			if (byte & hash[i])
				return (leading_zeroes);
			leading_zeroes++;
			byte >>= 1;
		}
	}
	return (leading_zeroes);
}
