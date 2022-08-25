//
// Created by Thomas Larsen on 8/25/22.
//

#include <stdlib.h>
#include "bufrecord.h"

struct BufferedRecordReader* NewBufferedRecordReader(FILE* stream, int fixedLen, char hasVarFields, long totalRecords){
    uint* recordBuffer;
    if (hasVarFields == 1) {
        recordBuffer = malloc(fixedLen+4+1000);
    } else {
        recordBuffer = malloc(fixedLen);
    }
    uint* lzfIn = malloc(bufferSize);
    uint* lzfOut = malloc(bufferSize);
    struct Lzf lzf = {lzfIn, lzfOut, 0, 0,0};

    struct BufferedRecordReader* reader = malloc(sizeof(struct BufferedRecordReader));
    reader->recordBuffer = recordBuffer;
    reader->recordBufferIndex = 0;
    reader->totalRecords = totalRecords;
    reader->stream = stream;
    reader->fixedLen = fixedLen;
    reader->hasVarFields = hasVarFields;
    reader->lzfIn = lzfIn;
    reader->lzfOut = lzfOut;
    reader->lzf = lzf;
    reader->lzfOutIndex = 0;
    reader->lzfOutSize = 0;
    reader->currentRecord = 0;
    return reader;
}

char BufferNextRecord(struct BufferedRecordReader* reader) {
    reader->currentRecord++;
    if (reader->currentRecord > reader->totalRecords) return 0;

    reader->recordBufferIndex = 0;
    return 1;
}