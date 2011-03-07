/*
 * properties_test.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"
#include "properties.h"
#include "crypto.h"

int tests_run = 0;

void test_set_property(struct esapi_ctx *ctx) {

	// Initialize/clear properties
	map_t *map = NULL;
	(void) fprintf(stdout, "Testing %s\n", __func__);
	TEST(set_property(&map, ctx, "foreground color", "yellow") != NULL);
}

void test_get_property(struct esapi_ctx *ctx) {

	// Initialize/clear properties
	map_t *map = NULL;
	(void) fprintf(stdout, "Testing %s\n", __func__);
	set_property(&map, ctx, "foreground color", "yellow");
	TEST(get_property(map, ctx, "foreground color") != 0);
	TEST(strcmp(get_property(map, ctx, "foreground color"), "yellow") == 0);
	TEST(get_property(map, ctx, "background color") == 0);
}

void test_store_properties(struct esapi_ctx *ctx) {
	// Initialize/clear properties
	map_t *map = NULL;
	(void) fprintf(stdout, "Testing %s\n", __func__);
	set_property(&map, ctx, "foreground color", "yellow");
	store_properties(map, "../configuration/encrypted.properties");
	// FIXME: What test?
}

void test_load_properties(struct esapi_ctx *ctx) {
	map_t *map = load_properties("../configuration/encrypted.properties");
	(void) fprintf(stdout, "Testing %s\n", __func__);
	TEST(get_property(map, ctx, "foreground color") != 0);
	TEST(strcmp(get_property(map, ctx, "foreground color"), "yellow") == 0);
}

int main(int argc, char **argv) {
	printf("Running unit test %s\n", argv[0]);

	struct esapi_ctx *ctx = load_security_context(
			"../configuration/ESAPI.properties");
	esapi_crypto_init(ctx);

	//	char *plaintext = "yellow";
	//	char *encrypted_value = esapi_encrypt(ctx, plaintext);
	//	(void)fprintf(stdout, "%s: Encrypted %s to %s\n", __FILE__, plaintext, encrypted_value);
	//	char *decrypted_value = esapi_decrypt(ctx, encrypted_value);
	//	(void)fprintf(stdout, "%s: Decrypted to %s\n", __FILE__, decrypted_value);

	test_set_property(ctx);
	test_get_property(ctx);
	test_store_properties(ctx);
	test_load_properties(ctx);

	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
