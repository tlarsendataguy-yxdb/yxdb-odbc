//
// Created by Thomas Larsen on 8/26/22.
//

#ifndef YXDB_ODBC_MIN_H
#define YXDB_ODBC_MIN_H

int minInt(int arg1, int arg2) {
    if (arg1 < arg2) {
        return arg1;
    }
    return arg2;
}

long minLong(long arg1, long arg2) {
    if (arg1 < arg2) {
        return arg1;
    }
    return arg2;
}

#endif //YXDB_ODBC_MIN_H
