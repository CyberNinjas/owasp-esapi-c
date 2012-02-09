/*
base64.c - c source to a base64 encoding algorithm implementation

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base64.h>

const int CHARS_PER_LINE = 72;
const int BREAK_LINES = 0;		// Do not break lines by default (RFC 3548)

static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool isbase64(char ch)
{
	return ch && strchr(encoding, ch) != NULL;
}

void base64_encode(const char *plaintext_in, size_t length_in, char *code_out, size_t length_out)
{
	(void) fprintf(stdout, "%s - Plaintext input: %s\n", __func__, plaintext_in);
	base64_encodestate state_in;
	base64_init_encodestate(&state_in);

	int len = base64_encode_block(plaintext_in, length_in, code_out, &state_in);
	int endlen = base64_encode_blockend(code_out + len, &state_in);
	code_out[len + endlen] = '\0';
	(void) fprintf(stdout, "%s - Encoded output: %s\n", __func__, code_out);
}

bool base64_decode(const char *code_in, size_t length_in, char *plaintext_out, size_t *length_out)
{
	(void) fprintf(stdout, "%s - Encoded input: %s\n", __func__, code_in);
	base64_decodestate state_in;
	base64_init_decodestate(&state_in);
	*length_out = base64_decode_block(code_in, length_in, plaintext_out, &state_in);
	(void) fprintf(stdout, "%s - Plaintext output length: %d\n", __func__, *length_out);
	plaintext_out[*length_out] = '\0';
	(void) fprintf(stdout, "%s - Plaintext output: %s\n", __func__, plaintext_out);

	return *length_out != -1;
}

size_t base64_encode_alloc(const char *plaintext_in, size_t length_in, char **code_out)
{
	size_t outlen = base64_encoded_size(length_in) + 1;

	*code_out = (char *) malloc(outlen);
	if (!*code_out) return outlen;

	base64_encode(plaintext_in, length_in, *code_out, outlen);

	return outlen - 1;
}

size_t base64_decode_alloc(const char *code_in, size_t length_in, char **plaintext_out, size_t *length_out)
{
	size_t needlen = base64_decoded_size(length_in);

	*plaintext_out = (char *) malloc(needlen);
	if (!*plaintext_out) return needlen;

	if (base64_decode(code_in, length_in, *plaintext_out, &needlen) == -1)
	{
		free(*plaintext_out);
		*plaintext_out = NULL;
		return false;
	}

	if (length_out)
		*length_out = needlen;

	return true;
}

size_t base64_encoded_size(size_t length)
{
	return (((length) + 2) / 3) * 4;
}

size_t base64_decoded_size(size_t length)
{
	return (length / 4) * 3 + 2;
}

void base64_init_encodestate(base64_encodestate* state_in)
{
	state_in->step = step_A;
	state_in->result = 0;
	state_in->stepcount = 0;
}

char base64_encode_value(char value_in)
{
	//static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (value_in > 63) return '=';
	return encoding[(int)value_in];
}

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in)
{
	const char* plainchar = plaintext_in;
	const char* const plaintextend = plaintext_in + length_in;
	char* codechar = code_out;
	char result;
	char fragment;

	result = state_in->result;

	switch (state_in->step)
	{
		while (1)
		{
	case step_A:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_A;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result = (fragment & 0x0fc) >> 2;
			*codechar++ = base64_encode_value(result);
			result = (fragment & 0x003) << 4;
	case step_B:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_B;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result |= (fragment & 0x0f0) >> 4;
			*codechar++ = base64_encode_value(result);
			result = (fragment & 0x00f) << 2;
	case step_C:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_C;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result |= (fragment & 0x0c0) >> 6;
			*codechar++ = base64_encode_value(result);
			result  = (fragment & 0x03f) >> 0;
			*codechar++ = base64_encode_value(result);

			++(state_in->stepcount);
			if (state_in->stepcount == CHARS_PER_LINE/4)
			{
				if (BREAK_LINES)
					*codechar++ = '\n';
				state_in->stepcount = 0;
			}
		}
	}
	/* control should not reach here */
	return codechar - code_out;

}

int base64_encode_blockend(char* code_out, base64_encodestate* state_in)
{
	char* codechar = code_out;

	switch (state_in->step)
	{
	case step_B:
		*codechar++ = base64_encode_value(state_in->result);
		*codechar++ = '=';
		*codechar++ = '=';
		break;
	case step_C:
		*codechar++ = base64_encode_value(state_in->result);
		*codechar++ = '=';
		break;
	case step_A:
		break;
	}
	if (BREAK_LINES)
		*codechar++ = '\n';

	return codechar - code_out;
}

int base64_decode_value(char value_in)
{
	static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
	static const char decoding_size = sizeof(decoding);
	value_in -= 43;
	if (value_in < 0 || value_in > decoding_size) return -1;
	return decoding[(int)value_in];
}

void base64_init_decodestate(base64_decodestate* state_in)
{
	state_in->step = step_a;
	state_in->plainchar = 0;
}

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in)
{
	const char* codechar = code_in;
	char* plainchar = plaintext_out;
	char fragment;

	*plainchar = state_in->plainchar;

	switch (state_in->step)
	{
		while (1)
		{
	case step_a:
			do {
				if (codechar == code_in+length_in)
				{
					state_in->step = step_a;
					state_in->plainchar = *plainchar;
					return plainchar - plaintext_out;
				}
				fragment = (char)base64_decode_value(*codechar++);
			} while (fragment < 0);
			*plainchar    = (fragment & 0x03f) << 2;
	case step_b:
			do {
				if (codechar == code_in+length_in)
				{
					state_in->step = step_b;
					state_in->plainchar = *plainchar;
					return plainchar - plaintext_out;
				}
				fragment = (char)base64_decode_value(*codechar++);
			} while (fragment < 0);
			*plainchar++ |= (fragment & 0x030) >> 4;
			*plainchar    = (fragment & 0x00f) << 4;
	case step_c:
			do {
				if (codechar == code_in+length_in)
				{
					state_in->step = step_c;
					state_in->plainchar = *plainchar;
					return plainchar - plaintext_out;
				}
				fragment = (char)base64_decode_value(*codechar++);
			} while (fragment < 0);
			*plainchar++ |= (fragment & 0x03c) >> 2;
			*plainchar    = (fragment & 0x003) << 6;
	case step_d:
			do {
				if (codechar == code_in+length_in)
				{
					state_in->step = step_d;
					state_in->plainchar = *plainchar;
					return plainchar - plaintext_out;
				}
				fragment = (char)base64_decode_value(*codechar++);
			} while (fragment < 0);
			*plainchar++   |= (fragment & 0x03f);
		}
	}
	/* control should not reach here */
	return plainchar - plaintext_out;
}
