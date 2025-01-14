#include "cli.h"


/**
 * wallet_load -	loads or creates a wallet
 *
 * @path:			path to the folder where the wallet is saved
 *
 * Return:			pair of key
 */
EC_KEY *wallet_load(char *path)
{
	EC_KEY *key = NULL;

	if (!path)
	{
		printf("No path specified.\n");
		return (NULL);
	}
	key = ec_load(path);
	if (key)
		printf("Key loaded.\n");
	else
	{
		key = ec_create();
		if (!key)
			printf("Failed to load or create a wallet.\n");
		else
			printf("Key created.\n");
	}
	return (key);
}


/**
 * wallet_save -	saves a wallet in path
 *
 * @key:			key to be saved
 *
 * @path:			path to save files in
 *
 * Return:			void
 */
void wallet_save(EC_KEY *key, char *path)
{
	if (!key || !path)
	{
		printf("No key or path specified.\n");
		return;
	}
	if (!ec_save(key, path))
		printf("Couldn't save the wallet.\n");
	else
		printf("Wallet saved.\n");
}
