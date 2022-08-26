#include <stdio.h>
#include "minunit.h"
#include "lzf_test.h"
#include "bufrecord_test.h"

int tests_run = 0;
int tests_passed = 0;

int main() {
    mu_run_test(test_lzf_small_control_values);
    mu_run_test(test_lzf_large_control_values);
    mu_run_test(test_lzf_large_control_value_length_of_7);
    mu_run_test(test_lots_of_records);
    mu_run_test(test_very_long_field);
    if (tests_run == tests_passed) printf("ALL %d TESTS PASSED", tests_run);
    return tests_run != tests_passed;
}
