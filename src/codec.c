/**
 * @file
 * A Codec module implementation.
 *
 * @since January 30, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "codec.h"

/*
 struct xml_entity {
 char c;
 char *entity;
 } xml_entities[] = {
 {'<', "&lt;"},
 {'>', "&gt;"},
 {'&', "&amp;"},
 {'"', "&quot"}
 };
 */

// Codecs

codec codec_unix = { "Unix", encode_unix_char, decode_unix_char };

codec *pcodec_unix = &codec_unix;

codec codec_windows = { "Windows", encode_windows_char, decode_windows_char };

codec *pcodec_windows = &codec_windows;

#if 0
codec codec_base64 = {"Base64",
	encode_base64_char,
	decode_base64_char
};
codec *pcodec_base64 = &codec_base64;

codec codec_oracle = {"Oracle",
	encode_oracle_char,
	decode_oracle_char
};
codec *pcodec_oracle = &codec_oracle;

codec codec_mysql = {"MySQL",
	encode_mysql_char,
	decode_mysql_char
};
codec *pcodec_mysql = &codec_mysql;
#endif

/*
 char *encode_xml(char *s) {
 char ch = *s;
 switch (ch) {
 case '<':
 return "&gt;";
 case '>':
 return "&lt;";
 case '&':
 return "&amp;";
 case '"':
 return "&quot;";
 }

 return strdup(s);
 }

 char *decode_xml(const char *s) {

 return 0;
 }
 */

bool hasNext(char *input, int index, char pushback) {
	if (pushback != -1)
		return true;
	if (input == NULL)
		return false;
	if (strlen(input) == 0)
		return false;
	if (index >= strlen(input))
		return false;
	return true;
}

char next(const char *input, int *index, char *pushback) {
//	printf("Getting next character to decode in %s at index: %d\n", input,
//			*index);
	if (*pushback != -1) {
		char save = *pushback;
		*pushback = -1;
		return save;
	}
	if (input == NULL)
		return -1;
	if (strlen(input) == 0)
		return -1;
	if (*index >= strlen(input))
		return -1;
	char c = input[*index];
	(*index)++;
	return c;
}

static const char *hex[256];

char *ctohex(char c) {
	char *s = (char *) malloc(6);

	sprintf(s, "%x", c);

	return s;
}

/**
 * Lookup the hex value of any character that is not alphanumeric.
 * @param c The character to lookup.
 * @return, return null if alphanumeric or the character code
 * 	in hex.
 */
char *nonatohex(char c) {
	return (char *) hex[(int) c];
}

/**
 * Initialize an array to mark which characters are to be encoded. Store the hex
 * string for that character to save time later. If the character shouldn't be
 * encoded, then store null.
 */
void codec_init() {
	for (int c = 0; c < 0xFF; c++) {
		if ((c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x5A) || (c >= 0x61
				&& c <= 0x7A)) {
			hex[c] = NULL;
		} else {
			hex[c] = ctohex((char) c);
		}
	}
}

