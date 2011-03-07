#include <stdio.h>
#include <regex.h>

#include "validate.h"

int main(void) {
	printf("is_valid return <%i>\n", is_valid("Hello", "[0-9a-zA-z]{1,6}",
			REG_EXTENDED | REG_NOSUB));

	return 0;
}
