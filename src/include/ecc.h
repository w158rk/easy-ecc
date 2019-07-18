#ifndef _EASY_ECC_H_
#define _EASY_ECC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
    the configuration parameters
 */

#define MAX_TRIES 16


/* ecc_make_key() function.
Create a public/private key pair.
    
Outputs:
    p_publicKey  - Will be filled in with the public key.
    p_privateKey - Will be filled in with the private key.

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int ecc_make_key(uint8_t p_publicKey[ECC_BYTES+1], uint8_t p_privateKey[ECC_BYTES]);

/* ecdh_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key.
Note: It is recommended that you hash the result of ecdh_shared_secret before using it for symmetric encryption or HMAC.

Inputs:
    p_publicKey  - The public key of the remote party.
    p_privateKey - Your private key.

Outputs:
    p_secret - Will be filled in with the shared secret value.

Returns 1 if the shared secret was generated successfully, 0 if an error occurred.
*/
int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES+1], const uint8_t p_privateKey[ECC_BYTES], uint8_t p_secret[ECC_BYTES]);

/* ecdsa_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and pass it in to
this function along with your private key.

Inputs:
    p_privateKey - Your private key.
    p_hash       - The message hash to sign.

Outputs:
    p_signature  - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES*2]);

/* ecdsa_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature values (r and s).

Inputs:
    p_publicKey - The signer's public key
    p_hash      - The hash of the signed data.
    p_signature - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/
int ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES+1], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES*2]);


/**! 
 * @brief the encryption of a message using ECC, the length of the plain text
 *              should be the multiple of the length of the curve bit
 * @return 0 if something wrong, 1 if no error 
 * @param[out] c the cipher 
 * @param[out] c_len the length of the cipher 
 * @param[in] m the message 
 * @param[in] m_len the length of the message 
 * @param[in] key the public key used to encrypt the message 
 */
int ecc_encrypt(uint8_t **c, size_t *c_len, uint8_t *m, size_t m_len, uint8_t *key);

/**! 
 * @brief the decryption of a message using ECC, the length of the cipher
 *              should be the multiple of the length of the curve bit
 * @return 0 if something wrong, 1 if no error 
 * @param[out] m the decrypted message
 * @param[out] m_len the length of the message
 * @param[in] c the ciphertext
 * @param[in] c_len the length of the ciphertext
 * @param[in] key the private key used to encrypt the message 
 */
int ecc_decrypt(uint8_t **m, size_t *m_len, uint8_t *c, size_t c_len, uint8_t *key);


#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _EASY_ECC_H_ */
