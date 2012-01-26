/**
 * @file
 * An ESAPI Configuration module implementation.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include <gcrypt.h>
#include <string.h>
#include <strings.h>
#include "base64.h"
#include "esapi.h"
#include "log.h"

#if !defined(strdup)
extern char *strdup (__const char *__s);
#endif

#define LINE_LEN	1024
#define PARSE_ERROR	0
#define PARSE_SUCCESS	1

char *value_delimiters = ", ";

char **_parse_csv(char *value) {
	int count = 0, i;
	char **result = 0;
	char *tmp;
	char *copy;

	if ((copy = strdup(value)) == 0) {
		(void) fprintf(stderr, "strdup failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	tmp = strtok(copy, value_delimiters);
	while (tmp) {
		count++;
		tmp = strtok(NULL, value_delimiters);
	}

	if (count) {
		if ((result = (char **) calloc(count, sizeof(char *))) == 0) {
			(void) fprintf(stderr, "calloc failure in %s\n", __func__);
			exit(EXIT_FAILURE);
		}

		tmp = strtok(copy, value_delimiters);

		char *token = value;
		for (i = 0; i < count; i++) {
			result[i] = token;
			token += strlen(token) + 1;
			token += strspn(token, value_delimiters);
		}
	}

	return result;
}

int _parse_csv_for_length(char *value) {
	int len = 0;
	char *tmp = 0;
	char *copy;

	if ((copy = strdup(value)) == 0) {
		(void) fprintf(stderr, "strdup failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	tmp = strtok(copy, value_delimiters);
	while ((tmp = strtok(NULL, value_delimiters))) {
		len++;
	}

	return len;
}

int _parse_boolean(char *value) {
	if (strcasecmp(value, "true") == 0) {
		return 1;
	}

	return 0;
}

int _parse_asymmetric_algo(const char *alg) {
	if (!strcmp(alg, "RSA")) {
		return GCRY_PK_RSA;
	} else if (!strcmp(alg, "DSA")) {
		return GCRY_PK_DSA;
	} else if (!strcmp(alg, "ELGAMAL")) {
		return GCRY_PK_ELG;
	}

	return 0;
}

int _parse_symmetric_algo(const char *alg) {
	if (!strcmp(alg, "3DES")) {
		return GCRY_CIPHER_3DES;
	} else if (!strcmp(alg, "AES128")) {
		return GCRY_CIPHER_AES128;
	} else if (!strcmp(alg, "AES192")) {
		return GCRY_CIPHER_AES192;
	} else if (!strcmp(alg, "AES256")) {
		return GCRY_CIPHER_AES256;
	} else if (!strcmp(alg, "TWOFISH256")) {
		return GCRY_CIPHER_TWOFISH;
	} else if (!strcmp(alg, "TWOFISH128")) {
		return GCRY_CIPHER_TWOFISH128;
	}

	return 0;
}

int _parse_symmetric_algo_mode(const char *value) {
	if (!strcmp(value, "ECB")) {
		return GCRY_CIPHER_MODE_ECB;
	} else if (!strcmp(value, "CBC")) {
		return GCRY_CIPHER_MODE_CBC;
	} else if (!strcmp(value, "CFB")) {
		return GCRY_CIPHER_MODE_CFB;
	} else if (!strcmp(value, "CTR")) {
		return GCRY_CIPHER_MODE_CTR;
	} else if (!strcmp(value, "OFB")) {
		return GCRY_CIPHER_MODE_OFB;
	}

	return 0;
}

int _parse_hash_algo(const char *alg) {
	if (!strcmp(alg, "SHA1")) {
		return GCRY_MD_SHA1;
	} else if (!strcmp(alg, "RIPEMD")) {
		return GCRY_MD_RMD160;
	} else if (!strcmp(alg, "MD5")) {
		return GCRY_MD_MD5;
	} else if (!strcmp(alg, "SHA224")) {
		return GCRY_MD_SHA224;
	} else if (!strcmp(alg, "SHA256")) {
		return GCRY_MD_SHA256;
	} else if (!strcmp(alg, "SHA384")) {
		return GCRY_MD_SHA384;
	} else if (!strcmp(alg, "SHA512")) {
		return GCRY_MD_SHA512;
	}
	return 0;
}

int _parse_config_entry(char *l, char *n, char *v) {
	for (; *l != '=' && *l != '\n' && *l != '\r' && *l != '\0'; l++, n++) {
		*n = *l;
	}

	*n = '\0';

	if (*l != '=')
		return PARSE_ERROR;
	else
		l++; // step over the '='

	for (; *l != '\n' && *l != '\r' && *l != '\0'; l++, v++) {
		*v = *l;
	}

	*v = '\0';

	return PARSE_SUCCESS;
}

/*
 * Load the context structure from a file.
 */
struct esapi_ctx *load_security_context(const char *infile) {
	FILE *fd;
	char line[LINE_LEN] = { '0' };
	char name[LINE_LEN] = { '0' };
	char value[LINE_LEN] = { '0' };
	struct esapi_ctx *ctx = 0;
	size_t outlen = 0;

	if ((ctx = (struct esapi_ctx *) malloc(sizeof(struct esapi_ctx))) == 0) {
		fprintf(stderr, "malloc error in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	// step 1: open up config file
	if ((fd = fopen(infile, "r")) == 0) {
		esapi_log_error(NULL, SECURITY_FAILURE,
				"Cannot open ESAPI configuration file %s in %s\n", infile, __func__);
		return 0;
	}

	// step 2: initialize default values for the context members
	ctx->print_properties = 0;
	ctx->master_key = "";
	ctx->master_salt = "";
	ctx->master_iv = "";
	ctx->allowed_exts = NULL;
	ctx->allowed_exts_len = 0;
	ctx->max_file_size = 100000;
	ctx->sym_algo = GCRY_CIPHER_AES256;
	ctx->sym_algo_mode = GCRY_CIPHER_MODE_CBC;
	memset(ctx->sym_key,'\0', sizeof(ctx->sym_key));
	ctx->hash_algo = GCRY_MD_SHA256;
	ctx->asym_algo = GCRY_PK_RSA;
	ctx->asym_pub_key = NULL;
	ctx->asym_priv_key = NULL;
	ctx->fips140 = 0;
	ctx->secure_mem = 0;
	ctx->allowed_login_attempts = 3;
//	ctx->quotas = NULL;
//	ctx->quota_len = 0;
	ctx->idle_timeout = 30;
	ctx->absolute_timeout = 240;

	// step 3: loop through lines, assigning members when recognized
	while (fgets(line, sizeof(line), fd) != NULL) {
		// Check for comments and empty lines
		if (line[0] != '#' && line[0] != '\r' && line[0] != '\n') {
			if (_parse_config_entry(line, name, value) == PARSE_ERROR) {
				fprintf(stderr, "Invalid line in ESAPI config file: '%s'", line);
			} else {
				int recognized = 1;
				if (!strcmp(name, "ESAPI.PrintProperties")) {
					ctx->print_properties = _parse_boolean(value);
				} else if (!strcmp(name, "Encryptor.MasterKey")) {
					base64_decode_alloc(value, strlen(value), &ctx->master_key,
							&outlen);
				} else if (!strcmp(name, "Encryptor.MasterSalt")) {
					ctx->master_salt = strdup(value);
				} else if (!strcmp(name, "Executor.AllowedExts")) {
					ctx->allowed_exts = _parse_csv(value);
					ctx->allowed_exts_len = _parse_csv_for_length(value);
				} else if (!strcmp(name, "Logger.MaxFileSize")) {
					ctx->max_file_size = atoi(value);
				} else if (!strcmp(name, "Encryptor.SymAlgo")) {
					ctx->sym_algo = _parse_symmetric_algo(value);
				} else if (!strcmp(name, "Encryptor.SymAlgoMode")) {
					ctx->sym_algo_mode = _parse_symmetric_algo_mode(value);
				} else if (!strcmp(name, "Encryptor.HashAlgo")) {
					ctx->hash_algo = _parse_hash_algo(value);
				} else if (!strcmp(name, "Encryptor.AsymAlgo")) {
					ctx->asym_algo = _parse_asymmetric_algo(value);
				} else if (!strcmp(name, "Encryptor.AsymPubKey")) {
					base64_decode_alloc(value, strlen(value),
							&ctx->asym_pub_key, &outlen);
				} else if (!strcmp(name, "Encryptor.AsymPrivKey")) {
					base64_decode_alloc(value, strlen(value),
							&ctx->asym_priv_key, &outlen);
				} else if (!strcmp(name, "Encryptor.FIPS140")) {
					ctx->fips140 = _parse_boolean(value);
				} else if (!strcmp(name, "Encryptor.SecureMem")) {
					ctx->secure_mem = _parse_boolean(value);
				} else if (!strcmp(name, "Authenticator.AllowedLoginAttempts")) {
					ctx->allowed_login_attempts = atoi(value);
				} else if (!strcmp(name, "Authenticator.IdleTimeout")) {
					ctx->idle_timeout = atoi(value);
				} else if (!strcmp(name, "Authenticator.AbsoluteTimeout")) {
					ctx->absolute_timeout = atoi(value);
				} else {
					recognized = 0;
					esapi_log_warn(NULL, EVENT_FAILURE, "Ignoring unrecognized ESAPI property name: %s", name);
				}
				if (recognized && ctx->print_properties) {
					printf("Loading property: %s=%s\n", name, value);
					esapi_log_error(NULL, SECURITY_SUCCESS, "Loading property: %s=%s", name, value);
				}
			}
		}
	}

	return ctx;
}

