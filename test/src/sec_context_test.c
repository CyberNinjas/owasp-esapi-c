#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "esapi.h"

int tests_run = 0;

void test_load_security_context(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	struct esapi_ctx *ctx = load_security_context(
			"../configuration/ESAPI.properties");
	TEST(ctx != 0);
	TEST(ctx->max_file_size == 500000000);
	TEST(ctx->allowed_login_attempts == 3);
//	TEST(ctx->quota_len == 0);
	TEST(ctx->idle_timeout == 30);
	TEST(ctx->absolute_timeout == 240);

}

int main(void) {
	test_load_security_context();
	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
