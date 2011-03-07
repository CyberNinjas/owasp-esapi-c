#include <stdio.h>

#include "minunit.h"
#include "crypto.h"

int tests_run = 0;

void test_esapi_encrypt(struct esapi_ctx *ctx) {
	(void) fprintf(stdout, "Testing %s\n", __func__);
	char *plaintext = "Hello";
	char *encrypted;
	char *decrypted;

	// FIXME: Why malloc these buffers?
	//   if((encrypted = (char *)malloc(1024)) == 0) {
	//      (void)fprintf(stderr, "malloc failure in %s\n", __func__);
	//      exit(EXIT_FAILURE);
	//   }

	//   if((decrypted = (char *)malloc(1024)) == 0) {
	//      (void)fprintf(stderr, "malloc failure in %s\n", __func__);
	//      exit(EXIT_FAILURE);
	//   }

	encrypted = esapi_encrypt(ctx, plaintext);
	decrypted = esapi_decrypt(ctx, encrypted);
	TEST(encrypted != 0);
	TEST(decrypted != 0);
	TEST(strcmp(plaintext, decrypted) == 0);
	if (encrypted != 0)
		free(encrypted);
	if (decrypted != 0)
		free(decrypted);
}

void test_esapi_decrypt(struct esapi_ctx *ctx) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	test_esapi_encrypt(ctx);
}

/*
 * Note: Unless we fix the seed, we cannot test the hashed value.
 */
void test_esapi_hash(struct esapi_ctx *ctx) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	TEST(ctx != 0);
	char *plaintext = "Hello";
	char *hashed = esapi_hash(ctx, plaintext);
	TEST(strcmp(plaintext, hashed) != 0);
	free(hashed);
}

/*
 * Note: Unless we fix the seed, we cannot test the random value.
 */
void test_esapi_fill_random_token(struct esapi_ctx *ctx) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	TEST(ctx != 0);
	TEST(ctx->max_file_size == 500000000);
	char buffer[9];
	esapi_fill_random_token(ctx, buffer, 8);
	buffer[8] = '\0';
	//fprintf(stdout, "Random token: %s in %s\n", buffer, __func__);
	TEST(strlen(buffer) == 8);	// @bug This test fails intermittently
	/* FIXME: Maybe also verify that buffer contains only chars from esapi_alphabet? */
}

/*
 char *esapi_encrypt(struct esapi_ctx *, char *);
 char *esapi_decrypt(struct esapi_ctx *, char *);
 char *esapi_hash(struct esapi_ctx *, const char *);
 void esapi_fill_random_token(struct esapi_ctx *, char *, int);
 char *esapi_sign(struct esapi_ctx *, char *);
 int esapi_verify_signature(struct esapi_ctx *, char *, char *);
 gcry_sexp_t convert_to_asymmetric_key(void *, int);
 */

int main(void) {
	struct esapi_ctx *ctx = load_security_context("../configuration/ESAPI.properties");
	esapi_crypto_init(ctx);
	test_esapi_encrypt(ctx);
	test_esapi_decrypt(ctx);
	test_esapi_hash(ctx);
	test_esapi_fill_random_token(ctx);
	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
