#pragma once
#include "utl/MemMgr.h"
#include <types.h>
#include "math/Rand2.h"
#include "os/Platform.h"
#include "utl/Symbol.h"
#include "utl/Str.h"
#include <vector>

namespace Hmx {
    class Object;
}

enum EofType {
    NotEof = 0,
    RealEof = 1,
    TempEof = 2
};

class BinStream {
public:
    /**
     * @brief The three seek types for BinStream::Seek.
     * @see Seek()
     */
    enum SeekType {
        kSeekBegin = 0,
        kSeekCur = 1,
        kSeekEnd = 2,
    };

    BinStream(bool littleEndian = false);
    virtual ~BinStream();
    /** Completely flush out this BinStream. */
    virtual void Flush() = 0;
    /** Get the current position of the BinStream. */
    virtual int Tell() = 0;
    /** Get the current End Of File type of the BinStream. */
    virtual EofType Eof() = 0;
    /** Get whether or not the BinStream has failed. */
    virtual bool Fail() = 0;
    /** Get this BinStream's name. */
    virtual const char *Name() const;
    virtual int ReadAsync(void *, int);
    /** Get whether or not this BinStream is cached. Interestingly only overwritten in
     * ChunkStream. */
    virtual bool Cached() const { return false; }
    /** Get this BinStream's current platform (Xbox, PS3, etc). Interestingly only
     * overwritten in ChunkStream. */
    virtual Platform GetPlatform() const { return kPlatformNone; }

    void Read(void *, int);
    void ReadEndian(void *, int);

    void Write(const void *, int);
    void WriteEndian(const void *, int);

    void EnableWriteEncryption();
    void EnableReadEncryption();
    void DisableEncryption();
    void ReadString(char *, int);

    void Seek(int, SeekType);
    bool AddSharedInlined(const class FilePath &);

    void PushRev(int, Hmx::Object *);
    int PopRev(Hmx::Object *);

    NEW_OVERLOAD("BinStream", 0x55);
    DELETE_OVERLOAD("BinStream", 0x55);

    BinStream &operator<<(const char *);
    BinStream &operator<<(const Symbol &);
    BinStream &operator<<(const String &);

    BinStream &operator>>(Symbol &);
    BinStream &operator>>(String &);

#define BS_READ_OP(typename)                                                             \
    BinStream &operator>>(typename &rhs) {                                               \
        ReadEndian(&rhs, sizeof(typename));                                              \
        return *this;                                                                    \
    }
#define BS_WRITE_OP(typename)                                                            \
    BinStream &operator<<(typename rhs) {                                                \
        WriteEndian(&rhs, sizeof(typename));                                             \
        return *this;                                                                    \
    }
    BS_READ_OP(int)
    BS_READ_OP(uint)
    BS_READ_OP(s16)
    BS_READ_OP(u16)
    BS_READ_OP(u32)
    BS_READ_OP(u64)
    BS_READ_OP(f32)
    BS_READ_OP(f64)

    BinStream &operator>>(char &out) {
        Read(&out, 1);
        return *this;
    }

    BinStream &operator>>(unsigned char &out) {
        Read(&out, 1);
        return *this;
    }

    BinStream &operator>>(bool &b) {
        unsigned char uc;
        *this >> uc;
        b = (uc != 0);
        return *this;
    }

    BS_WRITE_OP(int)
    BS_WRITE_OP(uint)
    BS_WRITE_OP(s16)
    BS_WRITE_OP(u16)
    BS_WRITE_OP(u32)
    BS_WRITE_OP(u64)
    BS_WRITE_OP(f32)
    BS_WRITE_OP(f64)

    BinStream &operator<<(char c) {
        Write(&c, 1);
        return *this;
    }

    BinStream &operator<<(unsigned char uc) {
        Write(&uc, 1);
        return *this;
    }

    BinStream &operator<<(bool b) {
        unsigned char uc = (b) ? 1 : 0;
        Write(&uc, 1);
        return *this;
    }

#undef BS_READ_OP
#undef BS_WRITE_OP
private:
    /** The specific implementation for reading from byte data.
     *  @param [in] data The data to read from.
     *  @param [in] bytes The number of bytes to read.
     */
    virtual void ReadImpl(void *data, int bytes) = 0;
    /** The specific implementation for writing to byte data.
     *  @param [in] data The data to write to.
     *  @param [in] bytes The number of bytes to read.
     */
    virtual void WriteImpl(const void *data, int bytes) = 0;
    /** The specific implementation for seeking within this BinStream.
     *  @param [in] offset The offset in the data to seek to.
     *  @param [in] type The Seek type.
     */
    virtual void SeekImpl(int offset, SeekType type) = 0;

protected:
    bool mLittleEndian; // 0x4
    Rand2 *mCrypto; // 0x8
    std::vector<struct ObjVersion> *mRevStack; // 0xc
};

inline unsigned short getHmxRev(int packed) { return packed; }
inline unsigned short getAltRev(int packed) { return (unsigned int)packed >> 0x10; }
inline int packRevs(unsigned short alt, unsigned short rev) {
    return (rev & ~0xFFFF0000) | (alt << 0x10);
}

class BinStreamRev {
public:
    BinStreamRev(BinStream &bs, int revs)
        : mRev(getHmxRev(revs)), mAltRev(getAltRev(revs)), mBinStream(bs) {}

    BinStreamRev &operator>>(bool &);
    operator BinStream &() const { return mBinStream; }

    int mRev;
    int mAltRev;
    BinStream &mBinStream;
};

// Note: `Allocator` here is actually the size/capacity type parameter on Wii.
// The name is based on Xbox 360 symbols, which show the allocator type instead.
template <class T, class Allocator>
BinStream &operator<<(BinStream &bs, const std::vector<T, Allocator> &vec) {
    bs << (int)vec.size();
    for (typename std::vector<T, Allocator>::const_iterator it = vec.begin();
         it != vec.end();
         it++) {
        bs << *it;
    }
    return bs;
}

/* WARNING: Inlined function: __savegprlr_27 */
/* class BinStream & __cdecl operator<<<class ObjectDir::Viewport,class
   stlpmtx_std::StlNodeAlloc<class ObjectDir::Viewport> >(class BinStream &,class
   stlpmtx_std::vector<class ObjectDir::Viewport,class stlpmtx_std::StlNodeAlloc<class
   ObjectDir::Viewport> > const &) */
