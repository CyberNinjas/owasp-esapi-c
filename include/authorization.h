#include <stdlib.h>
#include "user.h"

#ifndef _AUTHORIZATION_H
#define _AUTHORIZATION_H

int is_authorized_for_action(user *, const char *);

int is_authorized_for_data(user *, int);

int is_authorized_for_file(user *, const char *);

#endif
