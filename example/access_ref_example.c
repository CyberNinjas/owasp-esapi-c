#include <stdio.h>
#include <stdlib.h>
#include "access_ref.h"

int main(void) {
	char *values[] = {
			"apple",
			"orange",
			"tangelo",
			"peach",
			"strawberry"
	};
	char *keys[5];
	char *retrieved_values[5];
	int i;

	for (i = 0; i < 5; i++) {
		esapi_put_reference(esapi_get_unique_reference(values[i]), values[i]);
	}

	for (i = 0; i < 5; i++) {
		keys[i] = esapi_get_indirect_reference(values[i]);
	}
	printf("Access reference map:\n");

	for (i = 0; i < 5; i++) {
		retrieved_values[i] = esapi_get_direct_reference(keys[i]);
		printf("Entry %s:%s\n", keys[i], retrieved_values[i]);
	}
}
