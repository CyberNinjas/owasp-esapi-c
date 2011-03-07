#include <string.h>

#include "user.h"
#include "minunit.h"

int tests_run = 0;

void test_user(void) {
	user my_user; // local version

	(void) fprintf(stdout, "Running test %s\n", __func__);

	init_user(&my_user); // set local user structure to the initial value

	TEST(my_user.id == 0);
	TEST(get_current_id(&my_user) == 0l);
	TEST(strcmp(my_user.name, "UNKNOWN") == 0);
	TEST(strcmp(set_current_user(&my_user, 0), "UNKNOWN") == 0);
	TEST(strcmp(set_current_user(&my_user, "Tester"), "Tester") == 0);
	TEST(my_user.locked == 0);
	TEST(lock_user(&my_user) == 1);
	TEST(lock_user(&my_user) == 0);
	TEST(lock_user(0) == -1);
	TEST(unlock_user(&my_user) == 1);
	TEST(unlock_user(&my_user) == 0);
	TEST(unlock_user(0) == -1);
}

int main(void) {
	test_user();

	(void) fprintf(stderr, "%s: Pass <%i>, Fail <%i>\n", __FILE__, passed,
			failed);

	return (EXIT_SUCCESS);
}
