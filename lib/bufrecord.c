//
// Created by Thomas Larsen on 8/25/22.
//

#include <stdlib.h>
#include <libc.h>
#include "bufrecord.h"
#include "conversions.h"
#include "min.h"

struct BufferedRecordReader* NewBufferedRecordReader(FILE* stream, int fixedLen, char hasVarFields, long totalRecords){
    uint* recordBuffer;
    long recordBufferSize;
    if (hasVarFields == 1) {
        recordBufferSize = fixedLen + 4 + 1000;
        recordBuffer = malloc(recordBufferSize);
    } else {
        recordBufferSize = fixedLen;
        recordBuffer = malloc(recordBufferSize);
    }
    uint* lzfIn = malloc(bufferSize);
    uint* lzfOut = malloc(bufferSize);
    struct Lzf lzf = {lzfIn, lzfOut, 0, 0,0};

    struct BufferedRecordReader* reader = malloc(sizeof(struct BufferedRecordReader));
    reader->recordBuffer = recordBuffer;
    reader->recordBufferSize = recordBufferSize;
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

long readLzfBlockLength(struct BufferedRecordReader* reader) {
    unsigned long totalRead = fread(reader->lzfLengthBuffer, 1, 4, reader->stream);
    if (totalRead < 4) {
        return -1;
    }
    return (long) bytesToUint32(reader->lzfLengthBuffer);
}

long readNextLzfBlock(struct BufferedRecordReader* reader) {
    long lzfBlockLength = readLzfBlockLength(reader);
    if (lzfBlockLength < 0) {
        return -1;
    }
    long checkBit = lzfBlockLength & 0x80000000;
    if (checkBit > 0) {
        lzfBlockLength &= 0x7fffffff;
        return (long)fread(reader->lzfOut, 1, lzfBlockLength, reader->stream);
    }
    long readIn = (long)fread(reader->lzfIn, 1, lzfBlockLength, reader->stream);
    return Decompress(&reader->lzf, readIn);
}

long copyRemainingLzfOutToRecord(struct BufferedRecordReader* reader) {
    long remainingLzf = reader->lzfOutSize - reader->lzfOutIndex;
    memcpy(reader->recordBuffer+reader->recordBufferIndex, reader->lzfOut+reader->lzfOutIndex, remainingLzf);
    reader->recordBufferIndex += remainingLzf;
    return remainingLzf;
}

char readBufRecord(struct BufferedRecordReader* reader, long size) {
    while (size > 0) {
        if (reader->lzfOutSize == 0) {
            reader->lzfOutSize = readNextLzfBlock(reader);
            if (reader->lzfOutSize < 0) {
                return 0;
            }
        }

        while (size + reader->lzfOutIndex > reader->lzfOutSize) {
            size -= copyRemainingLzfOutToRecord(reader);
            reader->lzfOutSize = readNextLzfBlock(reader);
            reader->lzfOutIndex = 0;
        }

        long lenToCopy = minLong(reader->lzfOutSize, size);
        memcpy(reader->recordBuffer+reader->recordBufferIndex, reader->lzfOut+reader->lzfOutIndex, lenToCopy);
        reader->lzfOutIndex += lenToCopy;
        reader->recordBufferIndex += lenToCopy;
        size -= lenToCopy;
    }
    return 1;
}

char readVariableRecord(struct BufferedRecordReader* reader) {
    char success = readBufRecord(reader, reader->fixedLen+4);
    if (success == 0) {
        return 0;
    }
    int varLength = (int)bytesToUint32(reader->recordBuffer+reader->recordBufferIndex-4);
    if (reader->fixedLen + varLength + 4 > reader->recordBufferSize) {
        int newLength = (reader->fixedLen + 4 + varLength) * 2;
        uint* newBuffer = malloc(newLength);
        int copyTo = reader->fixedLen + 4;
        memcpy(newBuffer, reader->recordBuffer, copyTo);
        free(reader->recordBuffer);
        reader->recordBuffer = newBuffer;
        reader->recordBufferSize = newLength;
    }
    return readBufRecord(reader, varLength);
}

char BufferNextRecord(struct BufferedRecordReader* reader) {
    reader->currentRecord++;
    if (reader->currentRecord > reader->totalRecords) return 0;

    reader->recordBufferIndex = 0;

    if (reader->hasVarFields == 1) {
        return readVariableRecord(reader);
    } else {
        return readBufRecord(reader, reader->fixedLen);
    }
}

void Close(struct BufferedRecordReader* reader) {
    fclose(reader->stream);
}