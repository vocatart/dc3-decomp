#pragma once
#include "utl/HxGuid.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"
#include "types.h"
#include "os/Debug.h"
#include <vector>

class ArkHash {
public:
    ArkHash() : mHeap(0), mHeapEnd(0), mFree(0), mTable(0), mTableSize(0) {}
    ~ArkHash();
    int GetHashValue(const char *) const;
    void Read(BinStream &, int);
    int AddString(const char *);
    const char *operator[](int idx) const;

    char *mHeap; // 0x0
    char *mHeapEnd; // 0x4
    char *mFree; // 0x8
    char **mTable; // 0xc
    int mTableSize; // 0x10
};

class FileEntry {
    friend BinStream &operator>>(BinStream &, FileEntry &);

private:
    u64 mOffset;
    int mHashedName;
    int mHashedPath;
    int mSize;
    int mUCSize;

public:
    int HashedPath() const { return mHashedPath; }
    int Size() const { return mSize; }
};

const int preinitArk = 1;

class Archive {
public:
    enum Mode {
        kRead = 0,
        kWrite = 1,
    };
    Archive(const char *, int);
    ~Archive();
    bool GetFileInfo(const char *, int &, unsigned long long &, int &, int &);
    void
    Enumerate(const char *, void (*)(const char *, const char *), bool, const char *);
    const char *GetArkfileName(int) const;
    void GetGuid(HxGuid &) const;
    bool HasArchivePermission(int) const;
    void SetArchivePermission(int, const int *);
    int GetArkfileCachePriority(int) const;
    int GetArkfileNumBlocks(int) const;
    void SetLocationHardDrive();
    void Merge(Archive &);

    static bool DebugArkOrder();

    MEM_OVERLOAD(Archive, 100);

private:
    void Read(int);

    int mNumArkfiles; // 0x0
    std::vector<unsigned int> mArkfileSizes; // 0x4
    std::vector<String> mArkfileNames; // 0x10
    std::vector<int> mArkfileCachePriority; // 0x1c
    std::vector<FileEntry> mFileEntries; // 0x28
    ArkHash mHashTable; // 0x34
    String mBasename; // 0x48
    Mode mMode; // 0x50
    unsigned int mMaxArkfileSize; // 0x54
    bool mIsPatched; // 0x58
    HxGuid mGuid; // 0x5c
    const int *unk6c; // 0x6c
    int unk70; // 0x70
};

extern Archive *TheArchive;
void ArchiveInit();
