//
// Created by Thomas Larsen on 8/25/22.
//

#ifndef YXDB_ODBC_BUFRECORD_TEST_H
#define YXDB_ODBC_BUFRECORD_TEST_H

#include <stdio.h>
#include "minunit.h"
#include "lib/bufrecord.c"

static char* test_lots_of_records() {
    int headerSize = 512;
    FILE* stream = fopen("../test_files/LotsOfRecords.yxdb", "rb");
    uint* header = malloc(headerSize);
    fread(header, 1, headerSize, stream);
    int metaInfoSize = (int)*(int *)(header+80) * 2;
    long totalRecords = (long)*(long *)(header+104);

    fseek(stream, metaInfoSize, SEEK_CUR);
    mu_assertExpected("expected header size 134 but got %d\n", 134, metaInfoSize);

    struct BufferedRecordReader* reader = NewBufferedRecordReader(stream, 5, 0, totalRecords);

    int recordsRead = 0;
    while (BufferNextRecord(reader)) {
        recordsRead++;
    }
    mu_assertExpected("expected 100000 records but got %d", 100000, recordsRead);
    return 0;
}

#endif //YXDB_ODBC_BUFRECORD_TEST_H
