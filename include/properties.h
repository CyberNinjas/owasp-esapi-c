/**
 * @file
 * Encrypted Properties module header.
 */
/**
 * The EncryptedProperties module represents a properties file
 * where all the data is encrypted before it is added, and decrypted when it
 * retrieved. This interface can be implemented in a number of ways, the
 * simplest being wrapping some hashing container library and overloading
 * the behavior of the getProperty() and setProperty() functions. In all cases,
 * the master encryption key is given by the Encryptor.MasterKey property in
 * <b>ESAPI.properties</b> file.
 *
 * @since January 30, 2011
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "uthash.h"
#include "esapi.h"

#define MAX_PROPERTY_LINE_LENGTH 4096

/**
 * The maximum allowed size for a property key.
 */
#define KEY_LEN	64

struct Map_t {
	char key[KEY_LEN];
	char *val;
	UT_hash_handle hh; // hash handle
};

/**
 * A hashable key-value pair.
 */
typedef struct Map_t map_t;

/**
 * Get the plaintext property value for the given key from the given encrypted properties map.  The property
 * value is Base64 decoded and decrypted for the caller.
 *
 * @return A character array containing a plaintext property value, or NULL if the property was not found.
 */
extern char *get_property(map_t *, struct esapi_ctx *, const char *);

/**
 * Get the plaintext property value for the given key from the given encrypted properties map.  If the property
 * is not found, return the given default value.
 *
 * @return A character array containing a plaintext property value.
 */
extern char *get_property_or_default(map_t *, struct esapi_ctx *, const char *,
		const char *);

/**
 * Set the given property key-value pair in the given encrypted properties map.  The value is encrypted and
 * Base64 encoded before it is added to the map.
 *
 * @return A character array containing the plaintext property value that was set, or NULL on a failure.
 */
extern char *set_property(map_t **, struct esapi_ctx *, const char *,
		const char *);

/**
 * Get a list of all keys in the given encrypted properties map.
 *
 * @return An array of character arrays each containing a property key.
 */
extern char **keys(map_t *);

/**
 * Read encrypted properties from the given file.
 *
 * @return A head map_t structure pointer for the loaded properties.
 */
extern map_t *load_properties(const char *);

/**
 * Write encrypted properties to the given file.
 */
extern void store_properties(map_t *, const char *);

#endif /* PROPERTIES_H_ */
