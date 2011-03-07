#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "executor.h"

int tests_run = 0;

/*
 * Note: This test is necessarily platform (UNIX in this case) specific
 */
void test_esapi_execute(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	// FIXME: Need to find the correct directory for this file no matter what PWD is for the testing process.
	char *filename = "../configuration/ESAPI.properties";
	char *params[] = { "-n 1", filename };
	char *nave = "not_a_valid_executable";
	char *output = 0;

	TEST((output = esapi_execute("head", params, 2, ".", pcodec_unix, true)) !=0);
	TEST(strcmp(output, "#\n") == 0 || strcmp(output, "#\r\n") == 0);
	free(output);

	TEST((output = esapi_execute(nave, params, 2, ".", pcodec_unix, true)) != 0);
	TEST(strcmp(output, "\0") == 0);
	free(output);

	//   char *p[] = {"abc123", "test-file.txt"};
	//   TEST((output = esapi_execute("grep", p, 2, ".", pcodec_unix, true)) !=0);
	//   TEST(strcmp(output, "abc123\n") == 0);
	//   free(output);
}

int main(void) {
	test_esapi_execute();

	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
