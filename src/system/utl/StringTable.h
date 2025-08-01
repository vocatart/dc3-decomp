#pragma once
#include <vector>

class StringTable {
private:
    struct Buf {
        int size;
        char *chars;
    };

    std::vector<Buf> mBuffers; // 0x0
    char *mCurChar; // 0xc
    int mCurBuf; // 0x10

    void AddBuf(int);

public:
    // NEW_POOL_OVERLOAD(StringTable);
    // DELETE_POOL_OVERLOAD(StringTable);

    StringTable(int);
    ~StringTable();
    const char *Add(const char *);
    void Clear();
    void Reserve(int);
    int Size() const;
    int UsedSize() const;
};
