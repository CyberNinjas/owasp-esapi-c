#ifndef MINUNIT_H_
#define MINUNIT_H_
#include <stdio.h>	// fprintf
#include <string.h>	// strcmp

int passed = 0, failed = 0;

 #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
 #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

#define TEST(test)    \
   if (test) {  \
       ++passed;       \
       (void) fprintf(stdout, \
         " PASS: test %.d at line %.d\n", (passed + failed), __LINE__); \
   } else {      \
       ++failed; \
       (void) fprintf(stdout, \
         " FAIL: test %.d at line %.d\n", (passed + failed), __LINE__); \
   }

#endif /* MINUNIT_H_ */
