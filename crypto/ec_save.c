#include "hblk_crypto.h"

/**
 * ec_save - Saves an existing EC key pair on the disk.
 * @key: Points to the EC key pair to be saved on disk.
 * @folder: Path to the folder in which to save the keys
 * Return: 1 upon success, 0 on failure.
 */
int ec_save(EC_KEY *key, char const *folder)
{
    FILE *file_ptr;

    if (!key || !folder)
        return (0);

    /* Create folder if it doesn't exist */
    if (mkdir(folder, 0777) == -1)
    {
        if (errno != EEXIST)
            return (0);
    }

    /* Change process directory to folder */
    if (chdir(folder) != 0)
        return (0);

    /* Write the private key to private PEM file */
    file_ptr = fopen(PRI_FILENAME, "w");
    if (!file_ptr)
        return (0);
    if (PEM_write_ECPrivateKey(file_ptr, key, NULL, NULL, 0, NULL, NULL) == 0)
    {
        fclose(file_ptr);
        return (0);
    }
    fclose(file_ptr);

    /* Write the public key to public PEM file */
    file_ptr = fopen(PUB_FILENAME, "w");
    if (!file_ptr)
        return (0);
    if (PEM_write_EC_PUBKEY(file_ptr, key) == 0)
    {
        fclose(file_ptr);
        return (0);
    }
    fclose(file_ptr);

    return (1);
}
