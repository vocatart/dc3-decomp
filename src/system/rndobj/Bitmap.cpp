#include "rndobj/Bitmap.h"
#include "utl/CRC.h"
#include "utl/FileStream.h"
#include "utl/MemMgr.h"

unsigned char BITMAP_REV = 2;

int RndBitmap::NumMips() const {
    const RndBitmap *x = this;
    int i;
    for (i = 0; x->mMip; i++)
        x = x->mMip;
    return i;
}

int RndBitmap::PixelBytes() const { return mRowBytes * mHeight; }
int RndBitmap::DxtRowBytes() const { return mOrder & 0x38 ? mRowBytes * 4 : mRowBytes; }

unsigned char RndBitmap::PixelIndex(int i1, int i2) const {
    bool bb;
    int offset = PixelOffset(i1, i2, bb);
    if (mBpp == 8) {
        return mPixels[offset];
    } else if (bb) {
        return mPixels[offset] >> 4;
    } else {
        return mPixels[offset] & 0xF;
    }
}

void RndBitmap::SetName(const Hmx::CRC &crc) { mName = crc; }

BinStream &RndBitmap::LoadHeader(BinStream &bs, u8 &numMips) {
    u8 rev, h;
    u8 pad[32];
    bs.Tell();
    bs >> rev;
    if (rev > 1)
        bs >> mName;
    bs >> mBpp;
    if (rev != 0)
        bs >> mOrder;
    else {
        bs >> h;
        mOrder = h;
    }
    bs >> numMips;
    bs >> mWidth;
    bs >> mHeight;
    bs >> mRowBytes;

    int count;
    if (rev == 0) {
        count = 6;
    } else
        count = rev == 1 ? 0x13 : 0xF;
    bs.Read(pad, count);
    return bs;
}

BinStream &RndBitmap::SaveHeader(BinStream &bs) const {
    static u8 pad[0xf];
    bs << BITMAP_REV << mName << mBpp << (unsigned int)mOrder << (unsigned char)NumMips()
       << mWidth << mHeight;
    bs << mRowBytes;
    bs.Write(pad, 0xf);
    return bs;
}

BinStream &operator>>(BinStream &bs, tagBITMAPFILEHEADER &bmfh) {
    bs >> bmfh.bfSize;
    bs >> bmfh.bfReserved1;
    bs >> bmfh.bfReserved2;
    bs >> bmfh.bfOffBits;
    return bs;
}

BinStream &operator<<(BinStream &bs, const tagBITMAPFILEHEADER &bmfh) {
    bs << bmfh.bfSize << bmfh.bfReserved1 << bmfh.bfReserved2 << bmfh.bfOffBits;
    return bs;
}

BinStream &operator>>(BinStream &bs, tagBITMAPINFOHEADER &bmih) {
    bs >> bmih.biSize;
    bs >> bmih.biWidth;
    bs >> bmih.biHeight;
    bs >> bmih.biPlanes;
    bs >> bmih.biBitCount;
    bs >> bmih.biCompression;
    bs >> bmih.biSizeImage;
    bs >> bmih.biXPelsPerMeter;
    bs >> bmih.biYPelsPerMeter;
    bs >> bmih.biClrUsed;
    bs >> bmih.biClrImportant;
    return bs;
}

BinStream &operator<<(BinStream &bs, const tagBITMAPINFOHEADER &bmih) {
    bs << bmih.biSize << bmih.biWidth << bmih.biHeight << bmih.biPlanes << bmih.biBitCount
       << bmih.biCompression << bmih.biSizeImage << bmih.biXPelsPerMeter
       << bmih.biYPelsPerMeter << bmih.biClrUsed << bmih.biClrImportant;
    return bs;
}

void RndBitmap::Reset() {
    mName.Reset();
    mRowBytes = 0;
    mHeight = 0;
    mWidth = 0;
    mBpp = 32;
    mOrder = 1;
    mPalette = nullptr;
    mPixels = nullptr;
    if (mBuffer) {
        MemFree(mBuffer, __FILE__, 0x16C);
        mBuffer = nullptr;
    }
    RELEASE(mMip);
}

bool RndBitmap::LoadBmp(BinStream *bs) {
    unsigned short us;
    *bs >> us;
    if (us != 0x4D42) {
        MILO_NOTIFY("%s not BMP format", bs->Name());
        return false;
    } else {
        tagBITMAPFILEHEADER header;
        *bs >> header;
        return LoadDIB(bs, header.bfOffBits);
    }
}

void RndBitmap::SaveBmp(BinStream *file) const {
    MILO_ASSERT(file, 0x515);
    if (mOrder & 1) {
        MILO_NOTIFY("Order isn't kARGB");
    } else {
        SaveBmpHeader(file);
        SaveBmpPixels(file);
    }
}

void RndBitmap::SaveBmp(const char *cc) const {
    FileStream *file = new FileStream(cc, FileStream::kWrite, true);
    SaveBmp(file);
    delete file;
}
