/**
 * @file
 * A UNIX Command Shell Codec module implementation.
 *
 * @since January 30, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "codec.h"
#include "unix_codec.h"

char *unix_codec_name = "Unix";

/**
 * All possible chars for representing a number as a String
 */
const char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
		'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

/**
 * Utility to search a char[] for a specific char.
 *
 * @param c
 * @param array
 * @return
 */
static bool contains_char(char c, char array[]) {
	int i;
	for (i = 0; array[i]; i++) {
		if (c == array[i])
			return true;
	}
	return false;
}

/*
 * Returns backslash-encoded character
 *
 * @param immune
 */
char *encode_unix_char(char *tokenbuff, char ch) {

	// FIXME: Pass in a list of characters that should not be encoded
	const char *immune = "";

	// check for immune characters
	if (strchr(immune, ch)) {
		tokenbuff[0] = ch;
		tokenbuff[1] = '\0';
	} else {
		// check for alphanumeric characters
		char *hex = nonatohex(ch);
		if (hex == NULL) {
			tokenbuff[0] = ch;
			tokenbuff[1] = '\0';
		} else {
			tokenbuff[0] = '\\';
			tokenbuff[1] = ch;
			tokenbuff[2] = '\0';
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
char decode_unix_char(const char *input, int *index, char *pushback) {
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
	if (first != '\\') {
		//input.reset();
		*pushback = temp;
		*index = mark;
		return -1;
	}

	char second = next(input, index, pushback);
	return second;
}
