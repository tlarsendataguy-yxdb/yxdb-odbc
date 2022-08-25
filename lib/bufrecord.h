//
// Created by Thomas Larsen on 8/25/22.
//

#ifndef YXDB_ODBC_BUFRECORD_H
#define YXDB_ODBC_BUFRECORD_H

#include <stdio.h>
#include "uint.h"
#include "lzf.h"

struct BufferedRecordReader {
    uint* recordBuffer;
    int   recordBufferSize;
    int   recordBufferIndex;
    long  totalRecords;
    FILE* stream;
    int   fixedLen;
    char  hasVarFields;
    uint* lzfIn;
    uint* lzfOut;
    struct Lzf lzf;
    uint lzfLengthBuffer[4];
    int lzfOutIndex;
    int lzfOutSize;
    long currentRecord;
};

struct BufferedRecordReader* NewBufferedRecordReader(FILE* stream, int fixedLen, char hasVarFields, long totalRecords);

char BufferNextRecord(struct BufferedRecordReader*);

#endif //YXDB_ODBC_BUFRECORD_H
