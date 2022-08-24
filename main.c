#include <stdio.h>
#include "minunit.h"
#include "lib/lzf.c"

int tests_run = 0;
int tests_passed = 0;

int arrayEquals(const uint* arg1, const uint* arg2, int size) {
    int i = 0;
    while (i < size) {
        if (arg1[i] != arg2[i]) return 0;
        i++;
    }
    return 1;
}

static char* test_lzf_zero_input() {
    uint inData[6] = {4,1,2,3,4,5};
    uint outData[5];
    struct Lzf lzf = {(uint*)&inData,(uint*)&outData,0,0,0};

    int written = Decompress(&lzf, 6);
    uint expected[5] = {1,2,3,4,5};
    mu_assertExpected("expected 5 written bytes but got %d", written, 5);
    mu_assert("outData does not have the expected values%s", arrayEquals(outData, expected, 5));
    return 0;
}

int main() {
    mu_run_test(test_lzf_zero_input);
    if (tests_run == tests_passed) printf("ALL TESTS PASSED");
    return tests_run != tests_passed;
}
