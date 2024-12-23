#include "hblk_crypto.h"


/**
 * ec_load- func
 * @folder: char const *
 * Return: EC_KEY *
 */
EC_KEY *ec_load(char const *folder)
{
	int len = 0;
	char *path = NULL;
	FILE *fd = NULL;
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);
	len = strlen(folder) + 13;
	path = malloc(sizeof(char) * len);
	if (!path)
		return (NULL);
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);
	if (!sprintf(path, "%s/%s", folder, PUB_FILENAME))
		return (NULL);
	fd = fopen(path, "r");
	if (!fd || !PEM_read_EC_PUBKEY(fd, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(fd);
	if (!sprintf(path, "%s/%s", folder, PRI_FILENAME))
		return (NULL);
	fd = fopen(path, "r");
	if (!fd || !PEM_read_ECPrivateKey(fd, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(fd);
	free(path);
	return (key);
}
