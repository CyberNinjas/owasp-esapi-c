/**
 * @file
 * User module header.
 * @detail
 * Represents an application user or user account. There is quite a lot of information that an
 * application must store for each user in order to enforce security properly. There are also many rules that govern
 * authentication and identity management.
 * <P>
 * A user account can be in one of several states. When first created, a User should be disabled, not expired, and
 * unlocked. To start using the account, an administrator should enable the account. The account can be locked for a
 * number of reasons, most commonly because they have failed login for too many times. Finally, the account can expire
 * after the expiration date has been reached. The User must be enabled, not expired, and unlocked in order to pass
 * authentication.
 *
 * @since January 30, 2011
 */

#include <stdlib.h>

#ifndef _USER_H_
#define _USER_H_

#define NAME_MAX 64

/**
 * An application user account, including information required to enforce security on users properly.
 */
struct User_t {
	long id;
	char name[NAME_MAX];
	char **roles;
	int locked;
};

typedef struct User_t user;

/**
 * Get the user id for the given user.
 *
 * @return A long integer representing a unique identifier for the given user,
 * or -1 if the user is NULL.
 */
//extern long get_current_id(user *);

/**
 * Set the current user of this application to the given user.
 */
//extern char *set_current_user(user *, const char *);

/**
 * Clears the given user.
 */
//extern void init_user(user *);

/**
 * @internal
 * Adds the role to the user's list of roles.
 *
 * @asserts u is not null, role is not null
 */
//void add_role(user *u, char *role);

/**
 * @internal
 * Adds the roles to the user's list of roles.
 *
 * @asserts u is not null, roles is not null
 */
//void add_roles(user *u, char **roles);

/**
 * @internal
 * Removes the role passed in from the user's list of roles.
 */
//void remove_role(user *u, char *role);

/**
 * @internal
 * Checks if the user has the role specified.
 */
//int is_user_in_role(user *u, char *role);

/**
 * @internal
 * Adds the security event to the user's event list. It also
 * takes action if it is detected to be above a threshold.
 *
 * @asserts u is not null, role is not null
 */
//void add_event(user *u, char *event);

/**
 * @internal
 * Changes the user's password to the password passed in. If the passwords
 * don't match, then return fail.
 */
//int change_user_password(user *, char *new_passwd1, char *new_passwd2);

/**
 * @internal
 * Reset the user's password, and return the new password.
 */
//char *reset_password(user *);

/**
 * Locks the user.
 */
int lock_user(user *u);

/**
 * Unlocks the user.
 */
int unlock_user(user *u);

#endif /* _USER_H_ */
