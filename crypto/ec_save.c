#include "hblk_crypto.h"


/**
 * ec_save- func
 * @key: EC_KEY *
 * @folder: char const
 * Return: int
 */
int ec_save(EC_KEY *key, char const *folder)
{
	char *afolder = NULL, *bf = NULL;

	if (!key || !folder)
		return (0);
	bf = strdup(folder);
	if (!bf)
		return (0);
	afolder = strtok(bf, "/");
	while (afolder)
	{
		mkdir(afolder, 0777);
		chdir(afolder);
		afolder = strtok(NULL, "/");
	}
	free(bf);
	if (ec_save_public(key) || ec_save_private(key))
		return (0);
	return (1);
}


/**
 * ec_save_public- func
 * @key: EC_KEY *
 * Return: int
 */
int ec_save_public(EC_KEY *key)
{
	FILE *fd = NULL;

	fd = fopen(PUB_FILENAME, "w");
	if (!fd)
		return (1);
	PEM_write_EC_PUBKEY(fd, key);
	fclose(fd);
	return (0);
}


/**
 * ec_save_private- func
 * @key: EC_KEY *
 * Return: int
 */
int ec_save_private(EC_KEY *key)
{
	FILE *fd = NULL;

	fd = fopen(PRI_FILENAME, "w");
	if (!fd)
		return (1);
	PEM_write_ECPrivateKey(fd, key, NULL, NULL, 0, NULL, NULL);
	fclose(fd);
	return (0);
}
