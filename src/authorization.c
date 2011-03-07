/**
 * The Authorization module is stubbed out in this reference implementation.
 */

#include <stdlib.h>
#include "authorization.h"

int is_authorized_for_action(user *u, const char *action) {
	return 1;
}

int is_authorized_for_data(user *u, int data_type) {
	return 1;
}

int is_authorized_for_file(user *u, const char *file) {
	return 1;
}
