#pragma once
#include "utl/Str.h"
#include "utl/trie.h"
#include "utl/TextStream.h"

// size 0x65
#pragma pack(push, 1)
class AllocInfo {
private:
    int mBytes; // 0x0
    int mActual; // 0x4
    const char *mType; // 0x8
    void *mAddr; // 0xc
    signed char mHeapNum; // 0x10
    bool mPooled; // 0x11
    unsigned short unk12;
    unsigned char unk14;
    const char *unk15;
    int unk19;
    unsigned int unk1d;
    unsigned int unk21;
    int unk25[0x10]; // stack
    int unk61;

    static Trie *s_pTrie;

public:
    AllocInfo(int, int, const char *, void *, signed char, bool, unsigned char, const char *, int, String &, String &);
    ~AllocInfo();

    int Compare(const AllocInfo &) const;
    void FillStackTrace();
    void Validate() const;

    void PrintCsv(TextStream &) const;
    void PrintForReport(TextStream &) const;
    void Print(TextStream &) const;

    static bool bPrintCsv;
    static void SetPoolMemory(void *, int);
    static void *operator new(unsigned int);
    static void operator delete(void *);
};
#pragma pack(pop)

TextStream &operator<<(TextStream &, const AllocInfo &);
