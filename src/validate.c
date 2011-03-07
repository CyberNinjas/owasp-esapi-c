/**
 * @file
 * A Validation module implementation.
 *
 * @since January 30, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "validate.h"

/**
 * A precompiled regex cache entry.
 */
typedef struct element {
	char *pattern;
	regex_t r;
	struct element *prev;
	struct element *next;
} element;

/**
 * A cache of pre-compiled regex's stored as doubly-linked list.
 */
element *pattern_list = NULL;

int _cmp_ele(element *a, element *b) {
	return strcmp(a->pattern, b->pattern);
}

int contains_char(char *s, char c) {
	return (strchr(s, (int) c)) ? 1 : 0;
}

char *strip_chars(char *s, char *valid_chars) {
	char *buf;
	char *p = s;
	int i = 0;

	if ((buf = (char *) malloc(strlen(s) + 1)) == 0) {
		fprintf(stderr, "malloc failure in %s\n", __func__);
		exit(EXIT_FAILURE);
	}

	while (*p) {
		if (!contains_char(valid_chars, *p)) {
			buf[i++] = *p;
		}
		p++;
	}

	buf[i] = 0;

	return buf;
}

int is_valid(char *input, char *regex, int flags) {

	regex_t r;
	int compile_rc;
	static element search;
	element *found, *new_pattern;

	found = 0;

	if (!input) {
		return -1;
	}

	/*
	 * Check if we already compiled this regexp. If we have
	 * then use it. Otherwise, compile it and store it for
	 * later.
	 */

	search.pattern = regex;
	DL_SEARCH(pattern_list,found,&search,_cmp_ele);

	if (found) {
		r = found->r;
	} else {
		/* Add it to the list of already-compiled patterns. */
		compile_rc = regcomp(&r, regex, flags);

		if (compile_rc) {
			return compile_rc;
		}

		if ((new_pattern = (element *) malloc(sizeof(element))) == 0) {
			fprintf(stderr, "malloc failure in %s\n", __func__);
			exit(EXIT_FAILURE);
		}

		if ((new_pattern->pattern = strdup(input)) == 0) {
			fprintf(stderr, "strdup failure in %s\n", __func__);
			exit(EXIT_FAILURE);
		}

		new_pattern->r = r;
		DL_APPEND(pattern_list,new_pattern);
	}

	/* Execute the regular expression against the input. */

	free(new_pattern->pattern);
	free(new_pattern);

	return regexec(&r, input, 0, NULL, 0);
}
