//
// Created by Thomas Larsen on 8/25/22.
//

#ifndef YXDB_ODBC_CONVERSIONS_H
#define YXDB_ODBC_CONVERSIONS_H

#include <stdint.h>
#include "uint.h"

uint16_t bytesToUint16(uint* bytes){
    return (uint64_t)bytes[0] | ((uint64_t)bytes[1] << 8);
}

uint32_t bytesToUint32(uint* bytes) {
    return (uint64_t)bytes[0] | ((uint64_t)bytes[1] << 8) | ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24);
}

uint64_t bytesToUint64(uint* bytes) {
    return (uint64_t)bytes[0] | ((uint64_t)bytes[1] << 8) | ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24) | ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) | ((uint64_t)bytes[6] << 48) | ((uint64_t)bytes[7] << 56);
}

#endif //YXDB_ODBC_CONVERSIONS_H
