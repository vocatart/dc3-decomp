#include "os/Archive.h"
#include "math/Sort.h"
#include "os/Debug.h"
#include "os/File.h"
#include "utl/BinStream.h"
#include "utl/HxGuid.h"
#include "utl/MemMgr.h"
#include <cstdio>

bool gDebugArkOrder;
const int kArkBlockSize = 0x10000;

bool Archive::DebugArkOrder() { return gDebugArkOrder; }

Archive::~Archive() {}

Archive::Archive(const char *c, int i)
    : mNumArkfiles(0), mBasename(c), mMode(kRead), mMaxArkfileSize(0), mIsPatched(false),
      unk6c(0), unk70(0) {
    Read(i);
}

bool Archive::HasArchivePermission(int x) const {
    for (int i = 0; i < unk70; i++) {
        if (unk6c[i] == x)
            return true;
    }
    return false;
}

void Archive::SetArchivePermission(int i, const int *ci) {
    unk70 = i;
    unk6c = ci;
}

void Archive::GetGuid(HxGuid &guid) const { guid = mGuid; }

int ArkHash::AddString(const char *str) {
    int hashIdx = HashString(str, mTableSize);
    MILO_ASSERT(hashIdx < mTableSize, 0xB4);
    int idx = hashIdx;
    for (char *p = mTable[hashIdx]; p != nullptr;) {
        if (streq(p, str))
            return hashIdx;
        idx = (idx + 1) % mTableSize;
        if (idx == hashIdx) {
            MILO_FAIL("ERROR: Hash table full!!!");
        }
        p = mTable[idx];
    }
    hashIdx = idx;
    int len = strlen(str);
    MILO_ASSERT(mFree + len + 1 < mHeapEnd, 200);
    memcpy(mFree, str, len);
    MILO_ASSERT(hashIdx < mTableSize, 0xCA);
    mTable[hashIdx] = mFree;
    mFree += len + 1;
    return hashIdx;
}

int ArkHash::GetHashValue(const char *c) const {
    int hashIdx = HashString(c, mTableSize);
    MILO_ASSERT(hashIdx < mTableSize, 0xD4);
    while (mTable[hashIdx]) {
        if (streq(mTable[hashIdx], c))
            return hashIdx;
        if (++hashIdx == mTableSize)
            hashIdx = 0;
    }
    return -1;
}

const char *ArkHash::operator[](int idx) const {
    MILO_ASSERT(idx < mTableSize, 0xE7);
    return mTable[idx];
}

void ArkHash::Read(BinStream &bs, int len) {
    MemFree(mHeap);
    MemFree(mTable);
    int i40;
    bs >> i40;
    int iSizeBytes = i40 + len;
    char *mem = (char *)MemAlloc(iSizeBytes, __FILE__, 0x112, "ArkHash");
    mHeap = mem;
    mFree = mem + i40;
    mHeapEnd = mem + iSizeBytes;
    bs.Read(mem, iSizeBytes);
    memset(mFree, 0, mHeapEnd - mFree);
    bs >> mTableSize;
    mTable = (char **)MemAlloc(mTableSize * 4, __FILE__, 0x11A, "ArkHash");
    for (char *p = *mTable; p != mTable[mTableSize]; p++) {
        int offset;
        bs >> offset;
        p = offset == 0 ? nullptr : mHeap + offset;
    }
}

BinStream &operator>>(BinStream &bs, FileEntry &entry) {
    bs >> entry.mOffset >> entry.mHashedName >> entry.mHashedPath >> entry.mSize
        >> entry.mUCSize;
    return bs;
}

void Archive::Enumerate(
    const char *cc, void (*func)(const char *, const char *), bool b3, const char *c4
) {
    char buf[256];
    char buf2[256];
    if (c4) {
        const char *str = strstr(c4, ".dta");
        if (str) {
            sprintf(buf, "%s/gen/%s.dtb", FileGetPath(c4), FileGetBase(c4));
            c4 = buf;
            if (!b3) {
                sprintf(buf2, "%s/gen/", cc);
                cc = buf2;
            }
        }
    }
    for (std::vector<FileEntry>::iterator it = mFileEntries.begin();
         it != mFileEntries.end();
         ++it) {
        const char *cur = mHashTable[it->HashedPath()];
    }
}

int Archive::GetArkfileCachePriority(int arkfileNum) const {
    MILO_ASSERT(arkfileNum < mArkfileCachePriority.size(), 0x332);
    return mArkfileCachePriority[arkfileNum];
}

int Archive::GetArkfileNumBlocks(int file) const {
    return (mArkfileSizes[file] - 1) / kArkBlockSize + 1;
}

void Archive::SetLocationHardDrive() {
    for (int i = 0; i < mArkfileNames.size(); i++) {
    }
}

const char *Archive::GetArkfileName(int filenum) const {
    MILO_ASSERT(filenum < mArkfileNames.size(), 0x356);
    return mArkfileNames[filenum].c_str();
}
