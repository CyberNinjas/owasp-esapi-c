#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "minunit.h"
#include "validate.h"
#include "version.h"

#define REG_FLAGS   REG_EXTENDED | REG_NOSUB

int tests_run = 0;

void test_esapi_strip_chars(void) {

	(void) fprintf(stdout, "Testing %s\n", __func__);
	char *original = "Valid";
	char *stripped = 0;

	TEST((stripped = strip_chars(original, "id")) != 0);
	TEST(strcmp(stripped, "Val") == 0);
	free(stripped);

	TEST((stripped = strip_chars(original, "Val")) != 0);
	TEST(strcmp(stripped, "id") == 0);
	free(stripped);

	TEST((stripped = strip_chars(original, "a")) != 0);
	TEST(strcmp(stripped, "Vlid") == 0);
	free(stripped);

	TEST((stripped = strip_chars("1234567890", "24680")) != 0);
	TEST(strcmp(stripped, "13579") == 0);
	free(stripped);

	TEST((stripped = strip_chars("223344556677889900", "2345678")) != 0);
	TEST(strcmp(stripped, "9900") == 0);
	free(stripped);
}

void test_esapi_is_valid(void) {

	(void) fprintf(stdout, "Testing %s\n", __func__);
	TEST(is_valid("Valid", "[0-9a-zA-Z]{1,6}", REG_FLAGS) == 0);
	TEST(is_valid("Not valid", "[0-9a-zA-Z]{1,6}", 0) != 0 );
	TEST(is_valid("*&!@", "[0-9a-zA-Z]{1,6}", 0) != 0);
	TEST(is_valid(0, "[0-9a-zA-Z]{1,6}", 0) != 0);

}

int main(void) {
	test_esapi_strip_chars();
	test_esapi_is_valid();
	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}

