#include "utl/AllocInfo.h"
#include "os/Debug.h"
#include "trie.h"
#include "utl/TextStream.h"

Trie *AllocInfo::s_pTrie;
bool AllocInfo::bPrintCsv;

AllocInfo::AllocInfo(
    int i1,
    int i2,
    const char *cc1,
    void *v1,
    signed char sc,
    bool b,
    unsigned char uc,
    const char *cc2,
    int i3,
    String &str1,
    String &str2
)
    : mBytes(i1), mActual(i2), mType(cc1), mAddr(v1), mHeapNum(sc), mPooled(b), unk14(uc),
      unk15(cc2), unk19(i3), unk1d(s_pTrie->store(str1.c_str())),
      unk21(s_pTrie->store(str2.c_str())) {
    FillStackTrace();
}

AllocInfo::~AllocInfo() {
    MILO_ASSERT(s_pTrie, 0x6D);
    s_pTrie->remove(unk1d);
    s_pTrie->remove(unk21);
}

void AllocInfo::Validate() const { MILO_ASSERT(mPooled <= 1, 0xA5); }

void AllocInfo::PrintCsv(TextStream &ts) const {
    ts << MakeString("addr, 0x%lX, %s, bytes, %d ", (unsigned long)mAddr, mType, mBytes);
    MILO_ASSERT(s_pTrie, 0xC6);
    char buf1d[0x80];
    char buf21[0x80];
    ts << ", actual, " << mActual << ", heap, " << mHeapNum << ", " << unk15 << ", "
       << unk19 << ", " << s_pTrie->get(unk1d, buf1d, 0x80) << ", "
       << s_pTrie->get(unk21, buf21, 0x80);
    if (mPooled) {
        ts << ", pooled";
    }
}

void AllocInfo::Print(TextStream &ts) const {
    if (bPrintCsv)
        PrintCsv(ts);
    else {
        ts << MakeString(
            "(addr 0x%lX ) (type \"%s\") (bytes %d) ", (unsigned long)mAddr, mType, mBytes
        );
        if (mPooled)
            ts << "(pooled) ";
        ts << "(actual " << mActual << ") (heap_number " << mHeapNum << ") (location "
           << unk15 << " " << unk19 << ") ";
        ts << "(stack ";
        for (int i = 0; unk25[i] != 0 && i < 16; i++) {
            ts << unk25[i] << " ";
        }
        ts << ") ";
    }
}

TextStream &operator<<(TextStream &ts, const AllocInfo &info) {
    info.Print(ts);
    return ts;
}
