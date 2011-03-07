#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "access_ref.h"

int tests_run = 0;

void test_esapi_put_reference(void) {
	char *test_s;

	(void) fprintf(stdout, "Running test %s\n", __func__);

	// verify empty table
	TEST(HASH_CNT(hh_k, k_map) == 0);

	// verify that a new reference is successfully added
	TEST(esapi_put_reference("123", "100") == true);
	TEST(esapi_put_reference("456", "200") == true);
	TEST(esapi_put_reference("789", "300") == true);

	// verify unique keys are the only keys added
	TEST(esapi_put_reference("123", "100") == false);

	// verify correct number of entries are in each table
	TEST(HASH_CNT(hh_k, k_map) == 3);
	TEST(HASH_CNT(hh_v, v_map) == 3);

	// verify table can be retrieved by key
	TEST(strcmp(esapi_get_direct_reference("456"), "200") == 0);
	TEST((test_s = esapi_get_direct_reference("100")) == 0);

	// verify table can be retrieved by val
	TEST(strcmp(esapi_get_indirect_reference("300"), "789") == 0);

	// verify return on incorrect key
	TEST((test_s = esapi_get_indirect_reference("123")) == 0);

	// verify entry can be removed by key
	TEST(esapi_remove_direct_reference("456") == true);
	TEST(HASH_CNT(hh_k, k_map) == 2);
	TEST(HASH_CNT(hh_v, v_map) == 2);
	TEST((test_s = esapi_get_indirect_reference("200")) == 0);

	// verify entry can be removed by val
	TEST(esapi_remove_indirect_reference("100") == true);
	TEST(HASH_CNT(hh_k, k_map) == 1);
	TEST(HASH_CNT(hh_v, v_map) == 1);

	// verify returns on removed entries
	TEST((test_s = esapi_get_direct_reference("123")) == 0);
	TEST(esapi_remove_direct_reference("456") == false);

}

void test_esapi_get_unique_reference(void) {
	char *test_s;

	// verify esapi_get_unique_reference() produces a working key
	char *value = "This is a direct object reference";
	TEST(esapi_put_reference(esapi_get_unique_reference(value), value) == true);
	TEST((test_s = esapi_get_indirect_reference(value)) != 0);
}

int main(void) {

	test_esapi_put_reference();
	test_esapi_get_unique_reference();

	(void) fprintf(stderr, "%s: Pass <%i>, Fail <%i>\n", __FILE__, passed,
			failed);

	return (EXIT_SUCCESS);
}
