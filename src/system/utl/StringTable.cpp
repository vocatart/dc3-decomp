#include "utl/StringTable.h"

int StringTable::UsedSize() const {
    int size = 0;
    for (int i = 0; i < mBuffers.size(); i++) {
        const Buf &buf = mBuffers[i];
        if (i == mCurBuf) {
            size += (int)(mCurChar - buf.chars);
            break;
        }
        size += buf.size;
    }
    return size;
}

int StringTable::Size() const {
    int size = 0;
    for (int i = 0; i < mBuffers.size(); i++) {
        size += mBuffers[i].size;
    }
    return size;
}

StringTable::~StringTable() { Clear(); }

StringTable::StringTable(int i) : mCurChar(0), mCurBuf(-1) {
    if (i != 0)
        AddBuf(i);
}
