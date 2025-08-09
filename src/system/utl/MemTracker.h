#pragma once
#include "MemStats.h"
#include "obj/Data.h"
#include "utl/AllocInfo.h"
#include "utl/KeylessHash.h"
#include "utl/Str.h"

// size 0x1820c
class MemTracker {
private:
    static DataNode SpitAllocInfo(DataArray *);

public:
    MemTracker(int, int);

    void *mHashMem; // 0x0
    KeylessHash<void *, AllocInfo *> *mHashTable; // 0x4
    unsigned short unk8; // 0x8
    HeapStats mHeapStats[16]; // 0xc
    BlockStatTable mMemTable[2]; // 0x14c
    BlockStatTable mPoolTable[2]; // 0xc164
    int unk1817c; // 0x1817c
    void *unk18180; // 0x18180
    void *unk18184;
    void *unk18188;
    int unk1818c;
    int unk18190;
    bool unk18194;
    int unk18198;
    int unk1819c;
    int unk181a0;
    String unk181a4;
    String unk181ac;
    String unk181b4;

    // 2 BlockStatTables for mem
    // 2 BlockStatTables for pool
    // 0x18180: AllocInfoVec
    // 0x181bc: alloc info name
    // 0x1818c: mLog
};
