/**
 * The Authenticator module is stubbed out in this reference implementation.
 *
 * Consider writing an LDAP-based Authenticator reference implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gcrypt.h>
#include "authenticator.h"
#include "base64.h"
#include "esapi.h"

/*
 * Hashes the given password with a username as a salt. The third argument is
 * the hash algorithm to use. Passing zero means you get the default, SHA-256.
 */
char *esapi_hash_password(struct esapi_ctx *es_ctx, const char *real_pw) {

	char *hash;
	char *out;
	int hash_len;
	gcry_md_hd_t ctx;
	int algo;

	if (!es_ctx->hash_algo) {
		algo = GCRY_MD_SHA256;
	} else {
		algo = es_ctx->hash_algo;
	}

	hash_len = gcry_md_get_algo_dlen(algo);

	gcry_md_open(&ctx, algo, 0);
	gcry_md_write(ctx, es_ctx->master_salt, strlen(es_ctx->master_salt));
	gcry_md_write(ctx, (char *) real_pw, strlen(real_pw));
	hash = (char *) gcry_md_read(ctx, algo);

	if (!hash)
		return NULL;

	gcry_md_close(ctx);

	free(hash);

	size_t outlen = base64_encode_alloc(hash, hash_len, &out);

	if (out == NULL && outlen == 0 && hash_len != 0)
		return NULL;

	if (out == NULL)
		return NULL;

	puts(out);

	return out;
}

/*
 * Checks to see if the supplied login is correct.
 */
int esapi_login(const char *user_name, const char *pw) {
	if (!user_name || !pw) {
		return -1;
	}

	return 0;
}

/* 
 * Checks to see if a password is complex enough.
 */
int esapi_verify_password_strength(char *pw, int min, int max, int char_set_cnt) {
	int upper = 0;
	int lower = 0;
	int special = 0;
	char *p;

	if ((!pw && min > 0) || (pw && min != 0 && strlen(pw) < min)) {
		return ES_PW_TOO_SHORT;
	}

	if (max > 0 && strlen(pw) > max) {
		return ES_PW_TOO_LONG;
	}

	if (char_set_cnt > 0) {
		for (p = pw; *p; p++) {
			if (isupper(*p)) {
				upper = 1;
			} else if (islower(*p)) {
				lower = 1;
			} else {
				special = 1;
			}
		}
	}

	if (special + lower + upper < char_set_cnt) {
		return ES_PW_NOT_ENOUGH_CHAR_CLS;
	}

	return 0;
}

/*
 * Adds the given user to the persistence layer.
 */
int esapi_add_user(user *u) {

	return 0;
}

/*
 * Removes the given user from the persistence layer.
 */
int esapi_remove_user(user *u) {

	return 0;
}
