/**
 * @file
 * Codec module header.
 *
 * The Codec module defines a set of functions for encoding and decoding application level encoding schemes,
 * such as OS Command Shell escape encoding and Database interpreter escape encoding. Codecs are used in output
 * encoding and canonicalization.  Individual encoding schemes are implemented in separate modules and are
 * accessed though function pointers in the global codec definition variables.  The design of these codecs
 * allows for character-by-character decoding, which is necessary to detect double-encoding and the use of
 * multiple encoding schemes, both of which are techniques used by attackers to bypass validation and bury
 * encoded attacks in data.
 *
 * @since June 1, 2007
 * @see esapi_canonicalize()
 */

#include <stdlib.h>
#include <stdbool.h>

#ifndef _CODEC_H
#define _CODEC_H

#include "unix_codec.h"
#include "windows_codec.h"

/**
 *  A codec definition that includes the codec name and its encoder and decoder function pointers.
 */
typedef struct {
	char name[50];
	char *(*encode_char)(char *, char);
	char (*decode_char)(const char *, int *, char *);
} codec;

/**
 * A pointer to the global UNIX command shell codec definition.
 */
extern codec *pcodec_unix;

/**
 * A pointer to the global Windows command shell codec definition.
 */
extern codec *pcodec_windows;

extern bool hasNext(char *, int, char);
extern char next(const char *, int *, char *);
extern char *nonatohex(char c);
extern void codec_init();

#endif
