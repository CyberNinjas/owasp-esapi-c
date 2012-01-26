/**
 * @file
 * Cryptographic module header.
 *
 * This module provides some convenience methods for encryption, decryption, etc.
 * </p><p>
 * All the cryptographic operations use the default cryptographic properties as defined
 * in ESAPI.properties; e.g., default cipher transformation, default key size, default
 * IV type (where applicable), etc.
 *
 * @since January 30, 2011
 */

#include <gcrypt.h>

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "esapi.h"

/**
 * Do not include defintions for deprecated features
 */
#define GCRYPT_NO_DEPRECATED 1

/**
 * Do not define the shorthand macros
 */
#define GCRYPT_NO_MPI_MACROS 1

/**
 * \brief Encrypts the provided plaintext characters using the cipher transformation
 * specified by the property <code>Encryptor.CipherTransformation</code>
 * and the <i>master encryption key</i> as specified by the property
 * Encryptor.MasterKey as defined in the ESAPI.properties file.
 * </p><p>
 *
 * \param plaintext	A character array containing the plaintext to be encrypted.
 * \return A character array containing the resulting Base64 encoded cyphertext, or
 * NULL on any failure to encrypt.
 */
extern char *esapi_encrypt(struct esapi_ctx *, const char *);

/**
 * Decrypts the provided cyphertext string using the
 * <i>master encryption key</i> as specified by the property
 * Encryptor.MasterKey as defined in the ESAPI.properties file.
 * </p><p>

 * @param ciphertext A Base64 encoded cyphertext character array to be decrypted.
 * @return A character array containing the plaintext resulting from decrypting the specified
 * 		   ciphertext, or NULL on any failure to encrypt.
 */
extern char *esapi_decrypt(struct esapi_ctx *, const char *);

/**
 * Returns a string representation of the hash of the provided plaintext and
 * salt. The salt helps to protect against a rainbow table attack by mixing
 * in some extra data with the plaintext. Some good choices for a salt might
 * be an account name or some other string that is known to the application
 * but not to an attacker.
 * See <a href="http://www.matasano.com/log/958/enough-with-the-rainbow-tables-what-you-need-to-know-about-secure-password-schemes/">
 * this article</a> for more information about hashing as it pertains to password schemes.
 *
 * @param ctx
 * 		A valid esapi_ctx from which properly initialized cryptographic engine settings may be retrieved.
 * @param plaintext
 * 		the plaintext String to encrypt
 * @param salt
 *      the salt to add to the plaintext String before hashing
 *
 * @return	A character array containing the Base64 encoded encrypted hash of
 * the given plaintext, or NULL on any failure.
 *
 */
extern char *esapi_hash(struct esapi_ctx *, const char *);

/**
 * Returns a signature of the given message.
 * @param ctx
 * 		A valid esapi_ctx from which properly initialized cryptographic engine settings may be retrieved.
 * @param msg A character array containing the message
 * @param len The length of the message
 * @return A pointer to a gcry_sexp_t containing the signature
 */
extern gcry_sexp_t *esapi_sign(struct esapi_ctx *, void *, int);

/**
 * Generate a random secret key using a cryptographic algorithm and entropy source
 * appropriate for the generation of short term unique keys.
 * @param ctx	A properly initialized ESAPI context structure returned by @code
 *		load_security_context() @endcode
 * @param buff	A character array to hold the resulting key
 * @param len	The requested length of the key
 * @return 0 on success, or -1 on failure.
 */
extern int esapi_fill_random_token(struct esapi_ctx *, char *, int);

/**
 * Generate a random secret key using a cryptographic algorithm and entropy source
 * appropriate for the generation of long term cryptographic keys.
 * @param ctx	A properly initialized ESAPI context structure returned by @code
 *		load_security_context() @endcode
 * @param buff	A character array to hold the resulting key
 * @param len	The requested length of the key
 * @return 0 on success, or -1 on failure.
 */
extern int esapi_fill_crypto_key(struct esapi_ctx *, char *, int);

/**
 * Initializes the cryptographic engine.  This function must be called before
 * calling any of the other functions in this module.
 * @param ctx	A properly initialized ESAPI context structure returned by @code
 *		load_security_context() @endcode
 * @return 0 on success, or -1 on failure.
 */
extern int esapi_crypto_init(struct esapi_ctx *);

/**
 * @internal
 * Returns the validity of given signature for a given message.
 * @param ctx	A properly initialized ESAPI context structure returned by @code
 *		load_security_context() @endcode
 * @param msg	A character array holding the signed message data
 * @param m_len	The length of the message data
 * @param sig	A Base64-encoded signature for the given message
 * @return 0 on success, or -1 on failure.
 */
extern int esapi_verify_signature(struct esapi_ctx *, void *, int,
		gcry_sexp_t *);

/**
 * @internal
 */
extern gcry_sexp_t convert_to_asymmetric_key(void *, int);

#endif

