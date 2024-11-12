#include "hblk_crypto.h"

/**
 * ec_load - loads an EC key pair from the given folder
 * @folder: path to the folder containing key.pem (private key) and key_pub.pem (public key)
 * Return: EC_KEY* on success, or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
    FILE *fd = NULL;
    EC_KEY *key = NULL;

    // Check if the folder path is valid and change the directory
    if (!folder || chdir(folder) == -1) {
        perror("chdir failed");
        return NULL;
    }

    // Create a new EC key object with the specified curve
    key = EC_KEY_new_by_curve_name(EC_CURVE);
    if (!key) {
        perror("EC_KEY_new_by_curve_name failed");
        return NULL;
    }

    // Open the public key file (key_pub.pem) and read the key
    fd = fopen("key_pub.pem", "r");
    if (!fd || !PEM_read_EC_PUBKEY(fd, &key, NULL, NULL)) {
        perror("fopen or PEM_read_EC_PUBKEY failed");
        EC_KEY_free(key);
        return NULL;
    }
    fclose(fd);

    // Open the private key file (key.pem) and read the key
    fd = fopen("key.pem", "r");
    if (!fd || !PEM_read_ECPrivateKey(fd, &key, NULL, NULL)) {
        perror("fopen or PEM_read_ECPrivateKey failed");
        EC_KEY_free(key);
        return NULL;
    }
    fclose(fd);

    // Return the loaded EC key pair
    return key;
}
