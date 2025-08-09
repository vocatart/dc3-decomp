#pragma once

// size 0x14
class HeapStats {
public:
    HeapStats() : unk0(0), unk4(0), unk8(0), unkc(0), unk10(0) {}
    int unk0, unk4, unk8, unkc, unk10;
};

// size 0x18
class BlockStat {
public:
    const char *mName; // 0x0
    int unk4; // 0x4
    int unk8; // size?
    int unkc;
    int unk10;
    unsigned char unk14; // 0x14
};

// size 0x600c
class BlockStatTable {
private:
    BlockStat mStats[0x400]; // 0x0
    int unk6000; // 0x6000
    int mNumStats; // 0x6004
    bool unk6008; // 0x6008
public:
    BlockStatTable(bool = false);
    void Clear();
    void SortBySize();
    void SortByName();
    void Update(const char *, unsigned char, int, int);
    BlockStat &GetBlockStat(int);
};
