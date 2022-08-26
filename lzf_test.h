//
// Created by Thomas Larsen on 8/25/22.
//

#ifndef YXDB_ODBC_LZF_TEST_H
#define YXDB_ODBC_LZF_TEST_H

#include "lib/lzf.c"
#include "minunit.h"

int arrayEquals(const uint* arg1, const uint* arg2, int size) {
    int i = 0;
    while (i < size) {
        if (arg1[i] != arg2[i]) return 0;
        i++;
    }
    return 1;
}

static char* test_lzf_small_control_values() {
    uint inData[6] = {4,1,2,3,4,5};
    uint outData[5];
    struct Lzf lzf = {(uint*)&inData,(uint*)&outData,0,0,0};

    int written = Decompress(&lzf, 6);
    uint expected[5] = {1,2,3,4,5};
    mu_assertExpected("test_lzf_small_control_values: expected %d written bytes but got %d\n", 5, written);
    mu_assert("test_lzf_small_control_values: outData does not have the expected values\n", arrayEquals(outData, expected, 5));
    return 0;
}

static char* test_lzf_large_control_values() {
    uint inData[6] = {2, 1, 2, 3, 32, 1};
    uint outData[6];
    struct Lzf lzf = {(uint*)&inData,(uint*)&outData,0,0,0};

    int written = Decompress(&lzf, 6);
    uint expected[6] = {1, 2, 3, 2, 3, 2};
    mu_assertExpected("test_lzf_large_control_values: expected %d written bytes but got %d\n", 6, written);
    mu_assert("test_lzf_large_control_values: outData does not have the expected values\n", arrayEquals(outData, expected, 6));
    return 0;
}

static char* test_lzf_large_control_value_length_of_7() {
    uint inData[13] = {8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 224, 1, 8};
    uint outData[19];
    struct Lzf lzf = {(uint*)&inData,(uint*)&outData,0,0,0};

    int written = Decompress(&lzf, 13);
    uint expected[19] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1};
    mu_assertExpected("test_lzf_large_control_value_length_of_7: expected %d written bytes but got %d\n", 19, written);
    mu_assert("test_lzf_large_control_value_length_of_7: outData does not have the expected values\n", arrayEquals(outData, expected, 19));
    return 0;
}

#endif //YXDB_ODBC_LZF_TEST_H
