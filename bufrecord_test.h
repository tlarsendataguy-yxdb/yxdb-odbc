//
// Created by Thomas Larsen on 8/25/22.
//

#ifndef YXDB_ODBC_BUFRECORD_TEST_H
#define YXDB_ODBC_BUFRECORD_TEST_H

#include <stdio.h>
#include "minunit.h"
#include "lib/bufrecord.c"
#include "lib/conversions.h"

static char* test_lots_of_records() {
    int headerSize = 512;
    FILE* stream = fopen("../test_files/LotsOfRecords.yxdb", "rb");
    uint* header = malloc(headerSize);
    fread(header, 1, headerSize, stream);
    int metaInfoSize = (int)bytesToUint32(header+80) * 2;
    long totalRecords = (long)bytesToUint64(header+104);

    fseek(stream, metaInfoSize, SEEK_CUR);
    mu_assertExpected("expected header size %d but got %d\n", 134, metaInfoSize);

    struct BufferedRecordReader* reader = NewBufferedRecordReader(stream, 5, 0, totalRecords);

    int recordsRead = 0;
    while (BufferNextRecord(reader)) {
        int value = (int) bytesToUint32(reader->recordBuffer);
        recordsRead++;
        mu_assertExpected("test_lots_of_records: expected %d but got %d", recordsRead, value);
    }
    mu_assertExpected("test_lots_of_records: expected %d records but got %d", 100000, recordsRead);
    return 0;
}

static char* test_very_long_field() {
    int headerSize = 512;
    FILE* stream = fopen("../test_files/VeryLongField.yxdb", "rb");
    uint* header = malloc(headerSize);
    fread(header, 1, headerSize, stream);
    int metaInfoSize = (int)bytesToUint32(header+80) * 2;
    long totalRecords = (long)bytesToUint64(header+104);

    fseek(stream, metaInfoSize, SEEK_CUR);

    struct BufferedRecordReader* reader = NewBufferedRecordReader(stream, 6, 1, totalRecords);

    int recordsRead = 0;
    while (BufferNextRecord(reader)) {
        unsigned char value = reader->recordBuffer[0];
        recordsRead++;
        mu_assertExpected("test_very_long_field: expected %d but got %d", recordsRead, value);
    }
    mu_assertExpected("test_very_long_field: expected %d records but got %d", 3, recordsRead);
    return 0;
}

#endif //YXDB_ODBC_BUFRECORD_TEST_H
