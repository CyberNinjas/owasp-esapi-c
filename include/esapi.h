/**
 * @file
 * ESAPI Configuration module header.
 *
 * The configuration data is persisted in an ESAPI.properties file.  You can edit the
 * ESAPI.properties to set things like master keys and passwords, logging locations, error
 * thresholds, and allowed file extensions.
 * <p>
 * @warning Do not forget to update ESAPI.properties to change the master key and other security critical settings.
 *
 * @since January 30, 2011
 */

#ifndef _ESAPI_H
#define _ESAPI_H

//struct esapi_event_quota {
//	char *event;
//	int max;
//};

/**
 * All of the persistable settings used by the ESAPI.
 */
struct esapi_ctx {
	int print_properties;
	char *master_key;
	char *master_salt;
	char *master_iv;
	char **allowed_exts;
	int allowed_exts_len;
	long max_file_size;
	int sym_algo;
	int sym_algo_mode;
	char sym_key[128];
	int hash_algo;
	int asym_algo;
	char *asym_priv_key;
	char *asym_pub_key;
	int fips140;
	int secure_mem;
	int allowed_login_attempts;
//	struct esapi_event_quota *quotas;
//	int quota_len;
	int idle_timeout;
	int absolute_timeout;
};

/**
 * Load the ESAPI configuration. Never prints properties.
 * <p>
 * This is the function that returns a properly initialized esapi_ctx struct, which some ESAPI functions take
 * as a parameter.
 *
 */
extern struct esapi_ctx *load_security_context(const char *);

#endif
