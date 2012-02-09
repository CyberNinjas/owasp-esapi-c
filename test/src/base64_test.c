#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "base64.h"

int tests_run = 0;

void test_esapi_isbase64(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	TEST(isbase64('G') == true);
	TEST(isbase64('*') == false);
}

void test_esapi_base64_encode(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	char *input = "Hello";
	char encoded[32];
	char *expected = "SGVsbG8=";

	base64_encode(input, strlen(input), encoded, sizeof(encoded));

	TEST(strcmp(encoded, expected) == 0);
}

void test_esapi_base64_decode(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	char *input = "SGVsbG8=";
	size_t decoded_len = 32;
	char decoded[decoded_len];
	char *expected = "Hello";

	if (memset(decoded, '\0', decoded_len) != decoded) {
		(void) fprintf(stderr, "memset error in %s\n", __func__);
	} else {
		TEST(base64_decode(input, strlen(input), decoded, &decoded_len) == 1);
		TEST(strcmp(decoded, expected) == 0);
	}
}

int main(void) {
	test_esapi_isbase64();
	test_esapi_base64_encode();
	test_esapi_base64_decode();

	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed, failed);
	return (EXIT_SUCCESS);
}
