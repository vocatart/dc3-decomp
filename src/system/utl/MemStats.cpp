#include "utl/MemStats.h"
#include "os/Debug.h"

int SizeLess(const void *v1, const void *v2) {
    const BlockStat *b1 = (const BlockStat *)v1;
    const BlockStat *b2 = (const BlockStat *)v2;
    if (b1->unk8 < b2->unk8)
        return 1;
    // else...?
}

int NameLess(const void *v1, const void *v2) {
    const BlockStat *b1 = (const BlockStat *)v1;
    const BlockStat *b2 = (const BlockStat *)v2;
    return strcmp(b1->mName, b2->mName);
}

BlockStatTable::BlockStatTable(bool b) : unk6000(0x400), mNumStats(0), unk6008(b) {}

void BlockStatTable::Clear() { mNumStats = 0; }

void BlockStatTable::SortBySize() {
    qsort(mStats, mNumStats, sizeof(BlockStat), SizeLess);
}

void BlockStatTable::SortByName() {
    qsort(mStats, mNumStats, sizeof(BlockStat), NameLess);
}

BlockStat &BlockStatTable::GetBlockStat(int iStat) {
    MILO_ASSERT((0) <= (iStat) && (iStat) < (mNumStats), 0x37);
    return mStats[iStat];
}

void BlockStatTable::Update(const char *cc, unsigned char uc, int i3, int i4) {
    int idx = 0;
    for (; idx < mNumStats; idx++) {
        if (mStats[idx].unk14 == uc && (!unk6008 || mStats[idx].unk4 == i3)) {
            if (strcmp(mStats[idx].mName, cc) == 0) {
                if (!unk6008) {
                    mStats[idx].unk4 += i3;
                }
                mStats[idx].unk8 += i4;
                if (i3 >= mStats[idx].unkc) {
                    mStats[idx].unkc = i3;
                }
                mStats[idx].unk10++;
                return;
            }
        }
    }
    if (idx == mNumStats && mNumStats < unk6000) {
        mStats[mNumStats].mName = cc;
        mStats[mNumStats].unk14 = uc;
        mStats[mNumStats].unk4 = i3;
        mStats[mNumStats].unkc = i3;
        mStats[mNumStats].unk8 = i4;
        mStats[mNumStats].unk10 = 1;
        mNumStats++;
    } else {
        MILO_FAIL("Stack overflow in BlockStatTable!");
    }
}
