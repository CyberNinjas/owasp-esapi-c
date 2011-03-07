/**
 * @file
 * A local version of the non-Standard function strdup.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include <string.h>

char *( local_strdup)(const char *instr) {
	size_t instr_len = 1 + strlen(instr);
	char *newstr = (char *) malloc(instr_len);
	return newstr ? memcpy(newstr, instr, instr_len) : 0;
}

