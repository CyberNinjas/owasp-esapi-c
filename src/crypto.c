/**
 * @file
 * A Cryptographic module implementation.
 *
 * This implementation wraps the GnuPG project's libgrypt library, which
 * is available for many platforms.  This library is designed to meet
 * FIPS 140-2 requirements, and a version distributed for Redhat Linux
 * is certified by NIST to be FIPS 1402 compliant at level 1.
 *
 * @note If you need to handle long term cryptographic keys that protect
 * highly sensitive information, consider enabling Secure Memory in ESAPI
 * properties to minimize the likelihood of key exposure on the host computer.
 *
 * @see http://www.gnupg.org/download/
 * @see http://csrc.nist.gov/groups/STM/cmvp/documents/140-1/1401val2010.htm#1305
 *
 * @since January 30, 2011
 */

#include <gcrypt.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "crypto.h"
#include "base64.h"

static const char esapi_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

static int verbose;

#define BUF_SIZE        1024

char *esapi_encrypt(struct esapi_ctx *ctx, const char *s) {
    gcry_error_t err = GPG_ERR_NO_ERROR;
    gcry_cipher_hd_t hd;
    size_t blk_len = 0;
    char *enc_buff = 0;
    char *ret = 0;
    gcry_error_t key_len = 0;
    size_t in_len;
    int ret_len = 0;

    /* Validate before use */
    if(!ctx)
        return NULL;

    /* Set to empty string if invalid */
    if(!s)
        s = "";

    if ((err = gcry_cipher_open(&hd, ctx->sym_algo, ctx->sym_algo_mode, 0))
        != 0) {
        (void) fprintf(stderr, "gcry_cipher_open failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    if ((key_len = gcry_cipher_get_algo_keylen(ctx->sym_algo)) == 0) {
        (void) fprintf(stderr, "failed to get key length for %s in %s\n",
                       gcry_cipher_algo_name(ctx->sym_algo), __func__);
        return 0;
    }

    if (key_len > sizeof ctx->sym_key) {
        fprintf(stderr, "algo %d, keylength problem %d, in %s\n",
                ctx->sym_algo, key_len, __func__);
        return 0;
    }

    if (ctx->sym_key[0] == '\0') {
        /* FIXME: Need a secure generator here */
        for (unsigned int i = 0; i < key_len; i++) {
            ctx->sym_key[i] = i + (clock() & 0x7f);
        }
    }

    if ((blk_len = gcry_cipher_get_algo_blklen(ctx->sym_algo)) == 0) {
        (void) fprintf(stderr, "block length problem in %s\n", __func__);
        return 0;
    }

    if ((err = gcry_cipher_setkey(hd, ctx->sym_key, key_len)) != 0) {
        (void) fprintf(stderr, "gcry_cipher_setkey failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    if (ctx->master_iv) {
        if ((err = gcry_cipher_setiv(hd, ctx->master_iv, blk_len)) != 0) {
            (void) fprintf(stderr, "gcry_cipher_setiv failed:  %s/%s\n",
                           gcry_strsource(err), gcry_strerror(err));
            return 0;
        }
    }

    in_len = BUF_SIZE;

    if ((enc_buff = (char *) malloc(in_len)) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    if ((err = gcry_cipher_encrypt(hd, enc_buff, in_len, s, in_len)) != 0) {
        (void) fprintf(stderr, "gcry_cipher_encrypt failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    ret_len = base64_encode_alloc(enc_buff, in_len, &ret);

    if (ret == NULL && ret_len == 0 && in_len != 0) {
        free(enc_buff); enc_buff = NULL;
        return NULL; /* input was too long */
    } else if (ret == NULL) {
        free(enc_buff); enc_buff = NULL;
        return NULL; /* couldn't allocate output buffer */
    }

    if(enc_buff)
        free(enc_buff);

    if(hd)
        gcry_cipher_close(hd);

    return ret;
}

char *esapi_decrypt(struct esapi_ctx *ctx, const char *s) {
    gcry_error_t err = GPG_ERR_NO_ERROR;
    gcry_cipher_hd_t hd;
    size_t blk_len = 0;
    char *dec_buff = 0;
    char *ret = NULL;
    gcry_error_t key_len = 0;
    int ret_len = 0;
    size_t in_len = 0;
    bool dec_len = false;

    /* Validate before use */
    if(!ctx || !s)
        return NULL;

    if ((ret = (char *) malloc(BUF_SIZE)) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    if ((err = gcry_cipher_open(&hd, ctx->sym_algo, ctx->sym_algo_mode, 0))
        != 0) {
        (void) fprintf(stderr, "gcry_cipher_open failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    if ((key_len = gcry_cipher_get_algo_keylen(ctx->sym_algo)) == 0) {
        (void) fprintf(stderr, "failed to get key length for %s in %s\n",
                       gcry_cipher_algo_name(ctx->sym_algo), __func__);
        return 0;
    }

    if ((blk_len = gcry_cipher_get_algo_blklen(ctx->sym_algo)) == 0) {
        (void) fprintf(stderr, "failed to get block length for %s in %s\n",
                       gcry_cipher_algo_name(ctx->sym_algo), __func__);
        return 0;
    }

    if (ctx->sym_key[0] == '\0') {
        (void) fprintf(stderr, "%s missing key\n", __func__);
        return 0;
    }

    if ((err = gcry_cipher_setkey(hd, ctx->sym_key, key_len)) != 0) {
        (void) fprintf(stderr, "gcry_cipher_setkey failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    if (ctx->master_iv) {
        if ((err = gcry_cipher_setiv(hd, ctx->master_iv, blk_len)) != 0) {
            (void) fprintf(stderr, "gcry_cipher_setiv failed:  %s/%s\n",
                           gcry_strsource(err), gcry_strerror(err));
            return 0;
        }
    }

    if ((dec_buff = gcry_xmalloc(BUF_SIZE)) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    in_len = BUF_SIZE;

    dec_len = base64_decode_alloc(s, strlen(s), &dec_buff, &in_len);

    if (dec_buff == false && ret_len == 0 && strlen(dec_buff) != 0) {
        return NULL; /* input was too long */
    } else if (dec_buff == false) {
        return NULL; /* couldn't allocate output buffer */
    }

    if ((ret = (char *) malloc(in_len)) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    if ((err = gcry_cipher_decrypt(hd, ret, in_len, dec_buff, in_len)) != 0) {
        (void) fprintf(stderr, "gcry_cipher_decrypt failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        ret = 0;
    }

    return ret;
}

int esapi_crypto_init(struct esapi_ctx *ctx) {

    if (gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P)) {
        fprintf(stderr, "GCRYPT has already been initialized\n");
        return -1;
    } else if (!ctx) {
        fprintf(stderr, "Invalid ESAPI context\n");
        return -1;
    } else {

        /* Request FIPS 140-2 mode. GCRPYT docs don't state whether secure memory is used, so we        */
        /* explicitly set it. http://www.gnupg.org/documentation/manuals/gcrypt/Enabling-FIPS-mode.html */
        if (ctx->fips140) {
            gcry_control(GCRYCTL_FORCE_FIPS_MODE);
            ctx->secure_mem = 1;
        }

        if (ctx->secure_mem) {
            /* Version check should be the very first call because it
               makes sure that important subsystems are initialized. */
            if (!gcry_check_version(GCRYPT_VERSION)) {
                fputs("libgcrypt version mismatch\n", stderr);
                exit(EXIT_FAILURE);
            }

            /* We don't want to see any warnings, e.g. because we have not yet
               parsed program options which might be used to suppress such
               warnings. */
            gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

            /* ... If required, other initialization goes here.  Note that the
               process might still be running with increased privileges and that
               the secure memory has not been initialized.  */

            /* Allocate a pool of 16k secure memory.  This make the secure memory
               available and also drops privileges where needed.  */
            gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);

            /* It is now okay to let Libgcrypt complain when there was/is
               a problem with the secure memory. */
            gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

            /* ... If required, other initialization goes here.  */

        } else {
            gcry_control(GCRYCTL_SET_VERBOSITY, (int) verbose);

            if (!gcry_check_version(GCRYPT_VERSION)) {
                (void) fprintf(stderr, "GCRYPT version mismatch\n");
                exit(EXIT_FAILURE);
            }

            gcry_control(GCRYCTL_DISABLE_SECMEM, 0);

            gcry_control(GCRYCTL_ENABLE_QUICK_RANDOM, 0);
        }
        /* Tell Libgcrypt that initialization has completed. */
        gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    }
    return 0;
}

char *esapi_hash(struct esapi_ctx *ctx, const char *s) {
    int len = 0;
    void *digest = NULL;
    char *ret = 0;
    int ret_len = 0;

    /* Validate before use */
    if(!ctx)
        return NULL;

    /* Set to empty string if invalid */
    if(!s)
        s = "";

    len = gcry_md_get_algo_dlen(ctx->hash_algo);

    if ((digest = malloc(len)) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    gcry_md_hash_buffer(ctx->hash_algo, digest, s, strlen(s));

    ret_len = base64_encode_alloc(digest, len, &ret);

    if (ret == NULL && ret_len == 0 && len != 0) {
        free(digest); digest = NULL;
        return NULL; /* input was too long */
    } else if (ret == NULL) {
        free(digest); digest = NULL;
        return NULL; /* couldn't allocate output buffer */
    }

    if(digest)
        free(digest);

    return ret;
}

int esapi_fill_crypto_key(struct esapi_ctx *ctx, char *buff, int len) {
    gcry_random_level_t level = GCRY_VERY_STRONG_RANDOM;
    int i;

    /* Validate before use */
    if(!ctx || !buff || len < 0)
        return -1;

    /* WTF??? */
    if(!len)
        return 0;

    gcry_randomize(buff, len, level);

    for (i = 0; i < len; i++) {
        buff[i] = esapi_alphabet[(buff[i] % strlen(esapi_alphabet) - 1)];
    }

    return 0;
}

int esapi_fill_random_token(struct esapi_ctx *ctx, char *buff, int len) {
    gcry_random_level_t level = GCRY_STRONG_RANDOM;
    int i;

    /* Validate before use */
    if(!ctx || !buff || len < 0)
        return -1;

    /* WTF??? */
    if(!len)
        return 0;

    gcry_randomize(buff, len, level);

    for (i = 0; i < len; i++) {
        buff[i] = esapi_alphabet[(buff[i] % strlen(esapi_alphabet) - 1)];
    }

    return 0;
}

/**
 * Returns a signature of the given message.
 */
gcry_sexp_t *esapi_sign(struct esapi_ctx *ctx, void *msg, int len) {
    gcry_sexp_t *sig;
    static gcry_sexp_t data;
    gcry_error_t err;
    static gcry_sexp_t priv_key = NULL;

    /* Validate before use */
    if(!ctx || !msg || len < 0)
        return NULL;

    err = gcry_sexp_new(&data, msg, len, 0);

    if (err) {
        (void) fprintf(stderr, "gcry_sexp_new failed:  %s/%s\n",
                       gcry_strsource(err), gcry_strerror(err));
        return 0;
    }

    if ((sig = (gcry_sexp_t *) malloc(sizeof(gcry_sexp_t))) == 0) {
        (void) fprintf(stderr, "malloc failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    priv_key = convert_to_asymmetric_key(ctx->asym_priv_key, 0);
    if(!priv_key) {
        (void) fprintf(stderr, "failed to convert private key", __func__);
        exit(EXIT_FAILURE);
    }

    err = gcry_pk_sign(sig, data, priv_key);

    if (err) {
        free(sig);
        sig = NULL;
    }

    return sig;
}

/**
 * @internal
 */
gcry_sexp_t convert_to_asymmetric_key(void *buff, int key_len) {
    gcry_sexp_t empty = 0;
    return empty;
}

/**
 * Incomplete function implementation.
 *
 * @todo Fix to pass valid gcry_sexp_t parameters.
 */
int esapi_verify_signature(struct esapi_ctx *ctx, void *msg, int m_len,
                           gcry_sexp_t *sig) {
    gcry_error_t err;
    gcry_sexp_t data = NULL; // have to turn msg + len into this object
    gcry_sexp_t priv_key = NULL; // have to turn ctx->asym_priv_key into this
    err = gcry_pk_verify(*sig, data, priv_key);

    return (err == 0) ? 0 : -1;
}

/**
 * Incomplete function implementation.
 *
 * @todo Implement.
 */
char *encrypt_and_sign(struct esapi_ctx *ctx, void *msg, int len) {
    return NULL;
}

/**
 * Incomplete function implementation.
 *
 * @todo Implement.
 */
char *decrypt_and_verify(struct esapi_ctx *ctx, void *msg, int len) {
    return NULL;
}
