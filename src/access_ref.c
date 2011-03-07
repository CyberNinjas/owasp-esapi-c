/**
 * @file
 * An Access Reference Map module implementation.
 *
 * The reference implementation is based on 2 uthash hashmaps to support two-way
 * lookups (direct-to-indirect and indirect-to-direct).  The indirect reference
 * generator produces random keys.  A custom implementation might alternatively
 * provide a generator for incrementing integer keys.
 *
 * @todo To support more than one reference map per process, remove the global
 * 		map_t variables and change all functions to take a parameter that is an
 * 		array of two map_t handles.
 *
 * @todo The put function should be replaced by an add function that take only a
 * 		value param (plus the map).
 *
 * @todo esapi_fill_random_token() does not really need an ESAPI context parameter,
 * 		but its signature asks for one.
 *
 * @since January 30, 2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "access_ref.h"

/**
 * Value-to-key map
 */
map_t *v_map = NULL;

/**
 * Key-to-value map
 */
map_t *k_map = NULL;
map_t *s, *f;

bool _unique_keys(const char *k, const char *v) {
	map_t *loc_map1, *loc_map2;

	if (HASH_CNT(hh_k, k_map) != 0) { // initial entry, return true
		HASH_FIND(hh_k, k_map, k, strlen(k), loc_map1);
		HASH_FIND(hh_v, v_map, v, strlen(v), loc_map2);
		return (loc_map1 && loc_map2) ? false : true;
	}
	return true;
}

char *esapi_get_indirect_reference(const char *direct) {
	char *key = NULL;

	if (!direct) {
		return 0;
	} else {
		HASH_FIND(hh_v, v_map, direct, strlen(direct), f);
		if (f != NULL) {
			key = f->key;
		}
	}
	return key;
}

char *esapi_get_direct_reference(const char *indirect) {
	char *value = NULL;

	if (!indirect) {
		return 0;
	} else {
		HASH_FIND(hh_k, k_map, indirect, strlen(indirect), f);
		if (f != NULL) {
			value = f->val;
		}
	}
	return value;
}

bool esapi_remove_indirect_reference(const char *indirect) {
	bool ret_val = false;
	map_t *loc_map;

	if (indirect) {
		HASH_FIND(hh_v, v_map, indirect, strlen(indirect), loc_map);

		if (loc_map) {
			HASH_DELETE(hh_v, v_map, loc_map);
			HASH_DELETE(hh_k, k_map, loc_map);
			free(loc_map);
			ret_val = true;
		}
	}
	return ret_val;
}

bool esapi_remove_direct_reference(const char *direct) {
	bool ret_val = false;
	map_t *loc_map;

	HASH_FIND(hh_k, k_map, direct, strlen(direct), loc_map);

	if (loc_map) {
		HASH_DELETE(hh_v, v_map, loc_map);
		HASH_DELETE(hh_k, k_map, loc_map);
		free(loc_map);
		ret_val = true;
	}
	return ret_val;
}

bool esapi_put_reference(const char *k, const char *v) {
	bool ret_val = false;

	if (k && v) { // make sure the key and value are not NULL
		// allocate space, will be freed in the remove functions.
		if ((s = (map_t *) malloc(sizeof(map_t))) == 0) {
			(void) fprintf(stderr, "malloc failure in %s\n", __func__);
			exit(EXIT_FAILURE);
		} else if (_unique_keys(k, v)) { // add only unique keys
			strcpy(s->key, k);
			strcpy(s->val, v);
			// add key to structure
			HASH_ADD(hh_k, k_map, key, strlen(s->key), s);
			// add value as a key to structure
			HASH_ADD(hh_v, v_map, val, strlen(s->val), s);
			ret_val = true;
		}
	}
	return ret_val;
}

char *esapi_get_unique_reference() {
	char *reference = (char *) malloc(KEY_LEN);
	esapi_fill_random_token(NULL, reference, KEY_LEN);

	return reference;
}
