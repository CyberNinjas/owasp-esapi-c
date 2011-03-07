/**
 * @file
 * Canonicalization module header.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include <stdbool.h>
#include "codec.h"

#ifndef _CANONICALIZE_H
#define _CANONICALIZE_H

/**
 * Fully decode the given character array with the given codecs.  This function will
 * successfully decode strings that have been encoded with multiple codecs as well
 * as multiple times with any one codec.
 */
extern char *esapi_canonicalize(const char *, codec *, int, bool);

#endif
