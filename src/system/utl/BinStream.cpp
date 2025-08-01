#include "utl/BinStream.h"
#include "math/Rand.h"
#include "math/Rand2.h"
#include "obj/ObjVersion.h"
#include "os/Debug.h"
#include <vector>

#define BUF_SIZE 512

const char *BinStream::Name() const { return "<unnamed>"; }

BinStream::BinStream(bool b) : mLittleEndian(b), mCrypto(nullptr), mRevStack(nullptr) {}

void SwapData(const void *in, void *out, int size);

void BinStream::DisableEncryption() {
    MILO_ASSERT(mCrypto, 0xDC);
    RELEASE(mCrypto);
}

void BinStream::Seek(int offset, SeekType type) {
    MILO_ASSERT(!Fail(), 0x11F);
    MILO_ASSERT(!mCrypto, 0x122);
    SeekImpl(offset, type);
}

void BinStream::WriteEndian(const void *in, int size) {
    if (mLittleEndian) {
        u64 output;
        SwapData(in, &output, size);
        in = &output;
    }
    Write(in, size);
}

bool BinStream::AddSharedInlined(const class FilePath &) {
    MILO_FAIL("BinStream::AddSharedInlined is a PC dev tool only !!");
    return false;
}

BinStream &BinStream::operator<<(const char *str) {
    MILO_ASSERT(str, 0x60);
    int len = 0;
    const char *cc = str;
    while (*++cc)
        ;
    len = cc - str;
    len--;
    *this << len;
    Write(str, len);
    return *this;
}

BinStream &BinStream::operator<<(const Symbol &sym) {
    int len;
    MILO_ASSERT(len < BUF_SIZE, 0x6C);
    return *this;
}

BinStream &BinStream::operator<<(const class String &str) { return *this; }

void BinStream::EnableWriteEncryption() {
    MILO_ASSERT(!mCrypto, 0xC8);
    int i = RandomInt();
    *this << i;
    mCrypto = new Rand2(i);
}

void BinStream::ReadEndian(void *out, int size) {
    Read(out, size);
    if (mLittleEndian) {
        SwapData(out, out, size);
    }
}

void BinStream::ReadString(char *c, int i) {
    unsigned int a;
    *this >> a;
    if (a >= i)
        MILO_FAIL("String chars %d > %d", a + 1, i);
    Read(c, a);
    c[a] = 0;
}

BinStream &BinStream::operator>>(Symbol &sym) {
    char buf[BUF_SIZE];
    ReadString(buf, BUF_SIZE);
    sym = buf;
    return *this;
}
BinStream &BinStream::operator>>(String &str) {
    int siz;
    *this >> siz;
    str.resize(siz);
    Read((void *)str.c_str(), siz);
    return *this;
}

void BinStream::EnableReadEncryption() {
    MILO_ASSERT(!mCrypto, 0xC0);
    int i;
    *this >> i;
    mCrypto = new Rand2(i);
}

BinStream::~BinStream() {
    delete mCrypto;
    delete mRevStack;
}

void BinStream::PushRev(int revs, Hmx::Object *obj) {
    if (!mRevStack) {
        mRevStack = new std::vector<ObjVersion>();
    }
    mRevStack->push_back(ObjVersion(revs, obj));
}
