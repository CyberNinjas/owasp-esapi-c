#include <stdio.h>

#include "minunit.h"
#include "log.h"
#include "user.h"

int tests_run = 0;

void test_esapi_open_log(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	TEST(esapi_open_log() == 0);
}

void test_esapi_close_log(void) {
	(void) fprintf(stdout, "Testing %s\n", __func__);

	TEST(esapi_close_log() == 0);
}

void test_esapi_log_debug(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_debug(u, EVENT_SUCCESS, "Logging a debug message");

	free(u);
}

void test_esapi_log_trace(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_trace(u, EVENT_SUCCESS, "Logging a trace message");

	free(u);
}
void test_esapi_log_info(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_info(u, EVENT_SUCCESS, "Logging a info message");

	free(u);
}

void test_esapi_log_warn(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_warn(u, EVENT_FAILURE, "Logging a warn message");

	free(u);
}

void test_esapi_log_error(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_error(u, EVENT_FAILURE, "Logging a error message");

	free(u);
}

void test_esapi_log_fatal(void) {
	user *u = malloc(sizeof(user));

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_fatal(u, EVENT_FAILURE, "Logging a fatal message");

	free(u);
}

void test_esapi_log_debug_null_user(void) {

	(void) fprintf(stdout, "Testing %s\n", __func__);

	esapi_log_debug(NULL, EVENT_SUCCESS, "Logging a debug message");
}


int main(void) {

	test_esapi_open_log();
	test_esapi_log_debug();
	test_esapi_log_trace();
	test_esapi_log_info();
	test_esapi_log_warn();
	test_esapi_log_error();
	test_esapi_log_fatal();
	test_esapi_log_debug_null_user();
	test_esapi_close_log();

	(void) fprintf(stdout, "%s: Pass <%i>, Fail<%i>\n", __FILE__, passed,
			failed);
	return (EXIT_SUCCESS);
}
