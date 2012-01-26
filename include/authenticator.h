#include <stdio.h>
#include "user.h"
#include "esapi.h"

#ifndef _AUTHENTICATOR_H
#define _AUTHENTICATOR_H

#define ES_AUTH_NO_USER   -1
#define ES_AUTH_WRONG_PW  -2
#define ES_AUTH_EXPIRED   -3
#define ES_AUTH_LOCKED    -4
#define ES_AUTH_DISABLED  -5
#define ES_AUTH_INVALID   -6

#define ES_PW_TOO_LONG             -1
#define ES_PW_TOO_SHORT            -2
#define ES_PW_NOT_ENOUGH_CHAR_CLS  -3
#define ES_PW_INVALID              -4

/*
 * Hashes the given password with a username as a salt.
 */
char *esapi_hash_password(struct esapi_ctx *, const char *);

/*
 * Checks to see if the supplied login is correct.
 */
int esapi_login(const char *, const char *);

/* 
 * Checks to see if a password is complex enough.
 */
int esapi_verify_password_strength(char *, int, int, int);

/*
 * Adds the given user to the persistence layer.
 */
int esapi_add_user(user *);

/*
 * Removes the given user from the persistence layer.
 */
int esapi_remove_user(user *);

#endif
