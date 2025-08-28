#include "synth/Mic.h"
#include "os/Debug.h"
#include "obj/Data.h"
#include "utl/MemMgr.h"

void Mic::Set(const DataArray *data) {
    MILO_ASSERT(data, 0x12);
    SetGain(data->FindArray("gain")->Float(1));
    SetDMA(data->FindArray("dma")->Int(1) != 0);
    DataArray *compressorArr = data->FindArray("compressor");
    SetCompressor(compressorArr->Int(1) != 0);
    SetCompressorParam(compressorArr->Float(2));
}

void RingBuffer::Init(int size) {
    mSize = size;
    if (mBuffer) {
        MemFree(mBuffer, __FILE__, 0x2B);
        mBuffer = nullptr;
    }
    mBuffer = MemAlloc(size, __FILE__, 0x2C, "VirtualMic RingBuffer", 0x80);
    MILO_ASSERT(mBuffer, 0x2D);
    memset(mBuffer, 0, mSize);
    unkc = 0;
    unk10 = 0;
    unk4 = 0;
}
