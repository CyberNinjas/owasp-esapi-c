/**
 * @file
 * A Windows Command Shell Codec module implementation.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>
#include <string.h>

#include "codec.h"
#include "windows_codec.h"

char *windows_codec_name = "Windows";

/**
 * Returns a Windows shell encoded character
 *
 * @param tokenbuff A character array to hold the encoded character
 * @param ch	The character to encode
 */
char *encode_windows_char(char *tokenbuff, char ch) {

	// FIXME: Pass in a list of characters that should not be encoded
	const char *immune = "";

	// check for immune characters
	if (strchr(immune, ch)) {
		tokenbuff[1] = ch;
		tokenbuff[1] = '\0';
	} else {
		// check for alphanumeric characters
		char *hex = nonatohex(ch);
		if (hex == NULL) {
			tokenbuff[1] = ch;
			tokenbuff[1] = '\0';
		} else {
			tokenbuff[0] = '^';
			tokenbuff[1] = '\0';
		}
	}

	return tokenbuff;
}

/*
 * Returns the decoded version of the character starting at index, or
 * -1 if no decoding is possible.
 *
 * Formats all are legal both upper/lower case:
 *   \x - all special characters
 *
 */
char decode_windows_char(const char *input, int *index, char *pushback) {
	//input.mark();
	char temp = *pushback;
	char mark = *index;
	char first = next(input, index, pushback);
	if (first == -1) {
		//input.reset();
		*pushback = temp;
		*index = mark;
		return -1;
	}

	// if this is not an encoded character, return -1
	if (first != '^') {
		//input.reset();
		*pushback = temp;
		*index = mark;
		return -1;
	}

	char second = next(input, index, pushback);
	return second;
}
