#define mu_assertExpected(message, expected, actual) do { if (!(expected==actual)) {printf(message, actual); return message; }} while (0)
#define mu_assert(message, test) do { if (!(test)) {printf(message); return message; }} while (0)
#define mu_run_test(test) do {char *message = test(); tests_run++; \
                                if (!message) tests_passed++; } while (0)
extern int tests_run;
extern int tests_passed;