#include "hblk_crypto.h"
#include <stdio.h>
#include <string.h>

/**
 * ec_load - Load an EC key pair from disk
 * @folder: Path to the folder from which to load the keys
 * Return: Pointer to the EC key pair upon success, or NULL upon failure
 */
EC_KEY *ec_load(char const *folder)
{
    FILE *fp = NULL;
    char key_path[256] = {0};
    EC_KEY *key = NULL;

    if (!folder)
        return (NULL);

    /* Construct path to public key file */

    sprintf(key_path, "%s/%s", folder, PUB_FILENAME);

    /* Open and read public key */
    fp = fopen(key_path, "r");
    if (!fp || !PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
        return (NULL);
    fclose(fp);

    /* Construct path to private key file */ 
    sprintf(key_path, "%s/%s", folder, PRI_FILENAME);

    /* Open and read private key */ 
    fp = fopen(key_path, "r");
    if (!fp || !PEM_read_ECPrivateKey(fp, &key, NULL, NULL))
    {
        EC_KEY_free(key); /* free because allocated for public already */
		return (NULL);
    }
    fclose(fp);
    return (key);
}
