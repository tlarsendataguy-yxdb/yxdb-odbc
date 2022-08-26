//
// Created by Thomas Larsen on 8/23/22.
//

#ifndef YXDB_ODBC_LZF_H
#define YXDB_ODBC_LZF_H

#include "uint.h"

const int bufferSize = 262144;

struct Lzf {
    uint* inBuffer;
    uint* outBuffer;
    int   inIndex;
    int   outIndex;
    long  inLen;
};

long Decompress(struct Lzf*, long);

#endif //YXDB_ODBC_LZF_H
