/**
 * @file
 * An Encrypted Properties module implementation.
 *
 * @since January 30, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crypto.h"
#include "properties.h"
#include "log.h"
#include "user.h"

#define LF	0x0A
#define CR	0x0D

char *getline(char *s, int n, FILE *f) {
	char *line = fgets(s, n, f);

	if (line != NULL) {
		/* Strip any trailing newline */
		int len = strlen(line);
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
	}

	return line;
}

int putline(char *s, FILE *f) {
	return fputs(s, f);
}

char *get_property(map_t *map, struct esapi_ctx *ctx, const char *key) {

	char *value = NULL;

	map_t *entry = NULL;
	HASH_FIND(hh, map, key, strlen(key), entry);
	if (entry != NULL) {
		const char *encrypted_value = entry->val;
		if (encrypted_value != NULL) {
			value = esapi_decrypt(ctx, encrypted_value);
			if (value == NULL) {
				esapi_log_error(NULL, EVENT_FAILURE,
						"Property retrieval failure - couldn't decrypt property");
			}
		}
	}

	return value;
}

char *get_property_or_default(map_t *map, struct esapi_ctx *ctx,
		const char *key, const char *default_value) {

	char *value = get_property(map, ctx, key);
	if (value == NULL) {
		value = (char *) default_value;
	}

	return value;
}

char *set_property(map_t **maph, struct esapi_ctx *ctx, const char *key,
		const char *value) {

	char *encrypted_value;

	if (key == NULL) {
		esapi_log_error(NULL, EVENT_FAILURE, "Property name may not be null.");
	}
	if (value == NULL) {
		esapi_log_error(NULL, EVENT_FAILURE, "Property value may not be null.");
	}

	// Need error handling that logs "Property setting failure", "Couldn't encrypt property"
	encrypted_value = esapi_encrypt(ctx, value);
	if (encrypted_value == NULL) {
		esapi_log_error(NULL, EVENT_FAILURE,
				"Property setting failure - couldn't encrypt property");
	}

	map_t *entry;
	if ((entry = (map_t *) malloc(sizeof(map_t))) == 0) {
		(void) fprintf(stderr, "malloc failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	strcpy(entry->key, key);
	if ((entry->val = strdup(encrypted_value)) == 0) {
		(void) fprintf(stderr, "strdup failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	HASH_ADD(hh, *maph, key, strlen(entry->key), entry);

	return entry->val;
}

char **keys(map_t *map) {

	char **keys;
	int keylen = -1;

	map_t *entry, *tmp_entry;
	HASH_ITER(hh, map, entry, tmp_entry) {

		keylen = strlen(entry->key);
		*keys = (char *) malloc(keylen);
		// Error handling here
		strncpy(*keys, entry->key, keylen);
		keys[keylen] = '\0';
		(*keys)++;
	}

	return keys;
}

void clear_properties(map_t *map) {
	map_t *entry, *tmp_entry;

	HASH_ITER(hh, map, entry, tmp_entry) {
		HASH_DEL(map,entry); /* delete it */
		free(entry->val); /* free the dynamically sized value first */
		free(entry); /* free it */
	}
}

map_t *load_properties(const char *filename) {

	map_t *map = NULL;
	FILE *fh;
	char line[MAX_PROPERTY_LINE_LENGTH];
	//printf("Reading properties from file %s\n", filename);

	if ((fh = fopen(filename, "r")) != 0) {
		while (getline(line, MAX_PROPERTY_LINE_LENGTH, fh) != NULL) {
			//printf("Read properties file line %s\n", line);

			char *colon;

			if ((line[0] == '#') || (line[0] == '\0')) {
				continue;
			}
			colon = strchr(line, ':');
			if (colon != NULL) {
				*colon = '\0';
			}

			map_t *entry;
			if ((entry = (map_t *) malloc(sizeof(map_t))) == 0) {
				(void) fprintf(stderr, "malloc failure in %s\n", __func__);
				exit(EXIT_FAILURE);
			} else {
				strcpy(entry->key, line);
				//strcpy(entry->val, colon + 1);
				if ((entry->val = strdup(colon + 1)) == 0) {
					(void) fprintf(stderr, "strdup failure in %s\n", __func__);
					exit(EXIT_FAILURE);
				}

				HASH_ADD(hh, map, key, strlen(entry->key), entry);
			}

		}
		fclose(fh);
	}
	//printf("Finished reading properties from file\n");

	return map;
}

void store_properties(map_t *map, const char *filename) {

	FILE *fh;
	char line[MAX_PROPERTY_LINE_LENGTH];
	//printf("Writing properties to file %s\n", filename);
	//printf("Number of properties in map at %p is %d\n", map, HASH_CNT(hh, map));

	if ((fh = fopen(filename, "w+")) != 0) {
		map_t *entry, *tmp_entry;

		HASH_ITER(hh, map, entry, tmp_entry) {
			if (snprintf(line, MAX_PROPERTY_LINE_LENGTH, "%s:%s\n", entry->key, entry->val)
					== -1) {
				(void) fprintf(stderr, "property line too long in %s\n",
						__func__);
			} else {
				//printf("Writing properties file line %s\n", line);
				putline(line, fh);
			}
		}
		fflush(fh);
		fclose(fh);
	}
	//printf("Finished writing properties to file\n");
}

