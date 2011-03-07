/**
 * @file
 * Validation module header.
 * The Validation module defines a set of functions for canonicalizing and
 * validating untrusted input. Implementors should feel free to extend this
 * interface to accommodate their own data formats.
 *
 * Implementations must adopt a "whitelist" approach to validation where a
 * specific pattern or character set is matched. "Blacklist" approaches that
 * attempt to identify the invalid or disallowed characters are much more likely
 * to allow a bypass with encoding or other tricks.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include "utlist.h"
#include "codec.h"

#ifndef _VALIDATE_H
#define _VALIDATE_H

/**
 * Returns true if the given regular expression matches the given data.
 *
 * @param input A character array containing the user input data to validate.
 * @param type The regular expression name that maps to the actual regular
 * 		expression from "ESAPI.properties".
 * @param flags May be the bitwise-or of one or more of the following:
 * 		REG_EXTENDED, REG_ICASE, REG_NOSUB, REG_NEWLINE.
 * @return 0 on successful completion, or nonzero to indicate an error.
 */
extern int is_valid(char *, char *, int);

/**
 * Remove all characters from the given input character array that are also
 * found in the given valid characters array.
 *
 * @param input A character array containing the user input data to validate.
 * @param valid_chars A character array containing the characters to strip.
 */
extern char *strip_chars(char *, char *);

#endif
