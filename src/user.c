/**
 * The User module is stubbed out in this reference implementation.
 */

#include <string.h>

#include "user.h"

//void init_user(user *u) {
//	if (u) {
//		u->id = 0L;
//		strcpy(u->name, "UNKNOWN");
//		u->roles = 0;
//		u->locked = 0;
//	}
//}

// set name to UNKNOWN if set string is null
//char *set_current_user(user *u, const char *s) {
//	if (u == 0) {
//		return 0;
//	} else if (u) {
//		if (s) {
//			strcpy(u->name, s);
//		} else {
//			strcpy(u->name, "UNKNOWN");
//		}
//	}
//	return u->name;
//}

//long get_current_id(user *u) {
//	if (u) {
//		return u->id;
//	}
//	return -1;
//}

/*
 * Adds the role to the user's list of roles.
 *
 * @asserts u is not null, role is not null
 */
void add_role(user *u, char *role) {

	if (!u->roles) {
	} else {
	}

}

/*
 * Adds the roles to the user's list of roles.
 *
 * @asserts u is not null, roles is not null
 */
void add_roles(user *u, char **roles) {
	if (!u->roles) {

	} else {

	}
}

/*
 * Removes the role passed in from the user's list of roles.
 */
void remove_role(user *u, char *role) {

}

/*
 * Checks if the user has the role specified.
 */
int is_user_in_role(user *u, char *role) {
	return 0;
}

/*
 * Changes the user's password to the password passed in. If the passwords
 * don't match, then return fail.
 */
int change_user_password(user *u, char *new_passwd1, char *new_passwd2) {
	return 0;
}

/*
 * Reset the user's password, and return the new password.
 */
char *reset_password(user *u) {
	return 0;
}

/*
 * Locks the user.
 */
int lock_user(user *u) {
	if (u) {
		if (u->locked) {
			return 0;
		}
		u->locked = 1;
		return 1;
	}
	return -1;
}

int unlock_user(user *u) {
	if (u) {
		if (u->locked == 0) {
			return 0;
		}
		u->locked = 0;
		return 1;
	}
	return -1;
}
