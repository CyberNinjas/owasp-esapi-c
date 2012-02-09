/**
 * @file
 * Base64 Encoder/Decoder header.
 * <P>
 * <p>Encodes and decodes to and from Base64 notation.</p>
 * <p>This is part of the libb64 project, and has been placed in the public domain.</p>
 * <p>Homepage: <a href="http://sourceforge.net/projects/libb64">http://sourceforge.net/projects/libb64</a>.</p>
 *
 * @code
 *	char *input = "Hello";
 *	char encoded[32];
 *	base64_encode(input, strlen(input), encoded, sizeof(encoded));
 * @endcode
 *
 * @since January 25, 2012
 */

#ifndef BASE64_H
# define BASE64_H

/* Get size_t. */
# include <stddef.h>

/* Get bool. */
# include <stdbool.h>

/* Get malloc. */
# include <stdlib.h>

/* Get strchr. */
# include <string.h>


/**
 * Determines if the given character is within the Base64 encoding character set.
 */
extern bool isbase64(char ch);

/**
 * Base64 encodes the given input character array to the given output buffer.
 */
extern void base64_encode(const char *, size_t, char *, size_t);

extern size_t base64_encode_alloc(const char *, size_t, char **);


/**
 * Base64 decodes the given input character array to the given output buffer.
 */
extern bool base64_decode(const char *, size_t, char *, size_t *);

extern size_t base64_decode_alloc(const char *, size_t, char **, size_t *);


/*
cdecode.h - c header for a base64 decoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/

typedef enum
{
	step_a, step_b, step_c, step_d
} base64_decodestep;

typedef struct
{
	base64_decodestep step;
	char plainchar;
} base64_decodestate;

void base64_init_decodestate(base64_decodestate* state_in);

int base64_decode_value(char value_in);

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in);

size_t base64_decoded_size(size_t);

typedef enum
{
	step_A, step_B, step_C
} base64_encodestep;

typedef struct
{
	base64_encodestep step;
	char result;
	int stepcount;
} base64_encodestate;

void base64_init_encodestate(base64_encodestate* state_in);

char base64_encode_value(char value_in);

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in);

size_t base64_encoded_size(size_t);

int base64_encode_blockend(char* code_out, base64_encodestate* state_in);

#endif /* BASE64_H */

