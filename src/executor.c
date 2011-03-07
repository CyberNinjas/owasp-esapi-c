/**
 * @file
 * A Command Shell Executor module implementation.
 *
 * @since January 30, 2011
 */

#include <string.h>

#include "executor.h"
#include "log.h"

#define EXTRA_COMMANDS	7  // for "cd" and " && " in building command
/**
 * Determine whether the given path is canonicalized on this platform, using a POSIX method.
 */
int is_canonical(char *path) {
	int result;

	char *canonical_path = realpath(path, NULL);
	/* char *canonical_path = canonicalize_file_name(path, NULL); */
	if (canonical_path == NULL) {
		result = -1;
	} else {
		result = strcmp(path, canonical_path);
		free(canonical_path);
	}

	return result;
}

/**
 * Determine whether the given path is allowed by the configured security 
 * policy.
 */
int is_allowed(char *path) {
	/* FIXME: Check against an allowed list in the ESAPI configuration */
	return 1;
}

/**
 * The reference implementation sets the work directory, escapes the
 * parameters as per the Codec in use, and then executes the command
 * without using concatenation. The exact, absolute, canonical path of each
 * executable must be listed as an approved executable in the ESAPI properties.
 * The executable must also exist on the disk. All failures will be logged,
 * along with parameters if specified. Set the logParams to false if
 * you are going to invoke this interface with confidential information.
 */
char *esapi_execute(char *executable, char **params, int paramcount,
		char *workdir, codec *c, bool logParams) {
	FILE *fpipe = 0;
	int commandlen = 0; // for command line length
	char *output = 0;

	if (is_canonical(executable) == 0) {
		(void) fprintf(stderr, "Executable not a canonical path");
		return 0;
	}

	if (is_allowed(executable) == 0) {
		(void) fprintf(stderr, "Executable not an allowed path");
		return 0;
	}

	if (executable) {
		commandlen = strlen(executable);
	}

	if (workdir) {
		commandlen += strlen(workdir) + EXTRA_COMMANDS;
	}

	// escape any special characters in the parameters
	// FIXME: Need to construct a local char ** for the encoded params

	int i;
	int paramslen = 0;
	for (i = 0; i < paramcount; i++) {
		//    	char *param = params[i];
		//		params[i] = encodeForOS(c, param);
		paramslen += (strlen(params[i]) + 1);
	}
	commandlen += paramslen;

	// working directory must exist
	// set the command into the list and create command array

	char command[commandlen + 1];

	if (workdir) {
		if ((sprintf(command, "%s %s %s", "cd ", workdir, " && "))
				>= commandlen) {
			(void) fprintf(stderr, "command line build error\n");
			return 0;
		}
	}

	strcat(command, executable);

	for (i = 0; i < paramcount; i++) {
		char *param = params[i];
		strcat(command, " ");
		strcat(command, param);
	}

	if (logParams) {
		esapi_log_warn(NULL, EVENT_SUCCESS,
				"Initiating executable: %s %s in %s", executable, params,
				workdir);
	} else {
		esapi_log_warn(
				NULL,
				EVENT_SUCCESS,
				"Initiating executable: %s [sensitive parameters obscured] in %s",
				executable, workdir);
	}

	/* FIXME: I think we need to use exec */
	if ((fpipe = (FILE *) popen(command, "r")) == 0) {
		fprintf(stderr, "Problems with pipe\n");
		return 0;
	}

	char line[1024] = { 0 };

	if ((output = (char *) malloc(2048)) == 0) {
		(void) fprintf(stderr, "malloc failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	memset((char *)output, 0, sizeof(output));

	while (fgets(line, sizeof(line), fpipe)) {
		strcat(output, line);
	}

	pclose(fpipe);

	return output;
}
