//
// Created by Thomas Larsen on 8/23/22.
//

#include <stdlib.h>
#include <libc.h>
#include "lzf.h"
#include "min.h"

void reset(struct Lzf* lzf) {
    lzf->inIndex = 0;
    lzf->outIndex = 0;
}

void copyByteSequence(struct Lzf* lzf, uint ctrl) {
    int len = (int)ctrl + 1;
    if (lzf->outIndex + len > bufferSize) {
        exit(1);
    }
    memcpy(lzf->outBuffer+lzf->outIndex, lzf->inBuffer+lzf->inIndex, len);
    lzf->outIndex += len;
    lzf->inIndex += len;
}

int copyFromReferenceAndIncrement(struct Lzf* lzf, int reference, int size) {
    memcpy(lzf->outBuffer+lzf->outIndex, lzf->outBuffer+reference, size);
    lzf->outIndex += size;
    return reference + size;
}

void expandByteSequence(struct Lzf* lzf, uint ctrl) {
    int len = (int)(ctrl >> 5);
    int reference = lzf->outIndex - ((int)(ctrl&0x1f) << 8) - 1;

    if (len == 7) {
        len += (int)lzf->inBuffer[lzf->inIndex];
        lzf->inIndex++;
    }

    if (lzf->outIndex + len + 2 > bufferSize) {
        exit(1);
    }

    reference -= (int)lzf->inBuffer[lzf->inIndex];

    lzf->inIndex++;
    len += 2;

    while (len > 0) {
        int size = minInt(len, lzf->outIndex - reference);
        reference = copyFromReferenceAndIncrement(lzf, reference, size);
        len -= size;
    }
}

long Decompress(struct Lzf* lzf, long len) {
    lzf->inLen = len;
    reset(lzf);

    if (lzf->inLen == 0) {
        return 0;
    }

    while (lzf->inIndex < lzf->inLen) {
        uint ctrl = lzf->inBuffer[lzf->inIndex];
        lzf->inIndex++;

        if (ctrl < 32) {
            copyByteSequence(lzf, ctrl);
        } else {
            expandByteSequence(lzf, ctrl);
        }
    }
    return lzf->outIndex;
}
