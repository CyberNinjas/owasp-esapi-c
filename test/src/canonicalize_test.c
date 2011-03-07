#include <stdio.h>
#include "minunit.h"
#include "canonicalize.h"

/* Unpublished functions */
extern char *_codec_encode(codec *c, const char *s);
extern char *_codec_decode(codec *c, const char *s);

int tests_run = 0;

void test__codec_encode(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);
	const char *s = "Execute this: one\u00fftwo !@#$%^&*()";
//	printf("Encoding...\n");
	char *encoded = _codec_encode(pcodec_unix, s);
//	printf("Encoded to: %s\n", encoded);
	/* Test it */
}

void test__codec_decode(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);
	const char *s = "Execute this: one\u00fftwo !@#$%^&*()";
	char *decoded = _codec_decode(pcodec_unix, s);
//	printf("Decoded to: %s\n", decoded);
	/* Test it */

}

void test_esapi_canonicalize(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);
	const char *input = "Execute this: one\u00fftwo !@#$%^&*()";
	char *canonicalized = esapi_canonicalize(input, pcodec_unix, 1, 1);
//	printf("Encoded to: %s\n", canonicalized);
	/* Test it */
}

int main(void) {
	test__codec_encode();
	test__codec_decode();
	test_esapi_canonicalize();

	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
