/**
 * @file
 * A Canonicalization module implementation.
 *
 * @todo This module is unfinished.  When it is fixed, OS command shell
 * 		encoding can be performed by esapi_execute().
 *
 * @todo If it is feasible, the Base64 encoding module could be refactored
 * 		into a Base64 codec.
 *
 * @since January 30, 2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "codec.h"
#include "canonicalize.h"

#define GROW_LEN 32

// Encode algorithm:
// We must decode atoms and encode characters

//char decode_char(char *input, int *index, char *pushback) {
//	return next(input, index, pushback);
//}

char *straight_port_decode(codec *c, char *input) {
//	printf("Begin straight_port_decode\n");
	char pushback = -1;
	int bufflen = strlen(input)+1;
	char *buff = (char *) malloc(bufflen);
	int buffpos = 0;
	int index = 0;

	   /* The function pointer needed to canonicalize for the given codec. */
	   char (*decode_char)(const char *, int *, char *) = c->decode_char;

	int backstop = 0;
	while (hasNext(input, index, pushback)) {
		if (backstop++ > 30) exit(-1);
		char ch = decode_char(input, &index, &pushback);
//		   printf("Decoded character: %c\n", ch);
	      if ( buffpos + 1 > bufflen ) {
//		  	  	printf("Resizing output buffer\n");
	    	  bufflen = buffpos + 1 + GROW_LEN;
	         buff = realloc(buff, bufflen);
	      }

		 if (ch == -1) {
			 ch = next(input, &index, &pushback);
		 }
//		  	printf("Appending decoded character %c to %s at offset %d\n", ch, buff, buffpos);
		 buff[buffpos] = ch;
		 buffpos++;
		 buff[buffpos] = '\0';
	}

//	printf("End straight_port_decode returning value %s\n", buff);
	return buff;
}

//char encode_char(char *output, char c) {
//	printf("Encoding character: %c\n", c);
//	output[0] = c;
//	output[1] = '\0';
//
//	return output;
//}

char *straight_port_encode(codec *c, char *input) {
//	printf("Begin straight_port_encode\n");
	int bufflen = strlen(input)+1;
	char *buff = (char *) malloc(bufflen);
	char *tokenbuff = (char *) malloc(32);
	int tokenlen = 0;
	int buffpos = 0;

	   /* The function pointer needed to canonicalize for the given codec. */
	   char *(*encode_char)(char *, char) = c->encode_char;

	int inputlen = strlen(input);
	int i;
	for (i = 0; i < inputlen; i++) {
			char ch = input[i];
		   // FIXME: We could pass in a set of immune (unencodable) characters here
			encode_char(tokenbuff, ch);
	      tokenlen = strlen(tokenbuff);
	      if ( buffpos + tokenlen + 1 > bufflen ) {
//	  	  	printf("Resizing output buffer\n");
	    	  bufflen = buffpos + tokenlen + 1 + GROW_LEN;
	         buff = realloc(buff, bufflen);
	      }

//	  	printf("Appending encoded token %s to %s at offset %d\n", tokenbuff, buff, buffpos);
	      memcpy(buff+buffpos, tokenbuff, tokenlen);
	      buffpos += tokenlen;
	      buff[buffpos] = '\0';
	}

//	printf("End straight_port_encode returning value %s\n", buff);
	return buff;
}



char *_codec_decode(codec *c, char *s) {
	return straight_port_decode(c, s);
}

char *_codec_encode(codec *c, char *s) {
	return straight_port_encode(c, s);
}

/*
 * Decode an input according to an array of codecs. It's very likely that
 * the order of the codecs matters.
 */
char *esapi_canonicalize(const char *input, codec *codecs, int codec_count, bool strict) {
	if ( input == NULL ) {
		return NULL;
	}

	codec *current_codec;
	//codec *codecs = esapi_get_codecs();
	//int codec_count = esapi_get_codec_count();


	char *working = (char *)input;
	codec *codec_found = NULL;
	int codec_found_count = 0;
	int codec_mixed_count = 1;
	bool clean = false;
	while( !clean ) {
		clean = true;

		// try each codec and keep track of which ones work
		int i, offset;
		char *old;
		for(i = 0; i < codec_count; i++) {
			offset = sizeof(struct codec *) * i;
			current_codec = codecs + offset;
			old = working;
			working = _codec_decode( current_codec, working );
			if ( strcmp(old, working ) != 0 ) {
				if ( codec_found != NULL && codec_found != current_codec ) {
					codec_mixed_count++;
				}
				codec_found = current_codec;
				if ( clean ) {
					codec_found_count++;
				}
				clean = false;
			}
		}
	}

	// do strict tests and handle if any mixed, multiple, nested encoding were found
	if ( codec_found_count >= 2 && codec_mixed_count > 1 ) {
		if ( strict ) {
		    fprintf(stderr, "Input validation failure: Multiple (%dx) and mixed encoding (%dx) detected in %s\n", codec_found_count, codec_mixed_count, __func__);
		    return NULL;
//			throw new IntrusionException( "Input validation failure", "Multiple ("+ codec_found_count +"x) and mixed encoding ("+ codec_mixed_count +"x) detected in " + input );
		} else {
		    fprintf(stderr, "Multiple (%dx) and mixed encoding (%dx) detected in %s\n", codec_found_count, codec_mixed_count, __func__);
//			logger.warning( Logger.SECURITY_FAILURE, "Multiple ("+ codec_found_count +"x) and mixed encoding ("+ codec_mixed_count +"x) detected in " + input );
		}
	}
	else if ( codec_found_count >= 2 ) {
		if ( strict ) {
		    fprintf(stderr, "Input validation failure: Multiple encoding (%dx) detected in %s\n", codec_found_count, __func__);
		    return NULL;
//			throw new IntrusionException( "Input validation failure", "Multiple ("+ codec_found_count +"x) encoding detected in " + input );
		} else {
		    fprintf(stderr, "Multiple encoding (%dx) detected in %s\n", codec_found_count, __func__);
//			logger.warning( Logger.SECURITY_FAILURE, "Multiple ("+ codec_found_count +"x) encoding detected in " + input );
		}
	}
	else if ( codec_mixed_count > 1 ) {
		if ( strict ) {
		    fprintf(stderr, "Input validation failure: Mixed encoding (%dx) detected in %s\n", codec_mixed_count, __func__);
		    return NULL;
//			throw new IntrusionException( "Input validation failure", "Mixed encoding ("+ codec_mixed_count +"x) detected in " + input );
		} else {
		    fprintf(stderr, "Mixed encoding (%dx) detected in %s\n", codec_mixed_count, __func__);
//			logger.warning( Logger.SECURITY_FAILURE, "Mixed encoding ("+ codec_mixed_count +"x) detected in " + input );
		}
	}
	return working;
}

