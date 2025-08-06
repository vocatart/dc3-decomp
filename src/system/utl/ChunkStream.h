#pragma once
#include "utl/BinStream.h"

BinStream &MarkChunk(BinStream &);

class ChunkStream : public BinStream {
public:
    enum FileType {
        kRead = 0,
        kWrite = 1,
    };

    ChunkStream(const char *, FileType, int, bool, Platform, bool);
    virtual ~ChunkStream();
    virtual void Flush() {}
    virtual int Tell();
    virtual EofType Eof();
    virtual bool Fail();
    virtual const char *Name() const;
    virtual bool Cached() const;
    virtual Platform GetPlatform() const;

private:
    virtual void ReadImpl(void *, int);
    virtual void WriteImpl(const void *, int);
    virtual void SeekImpl(int, SeekType);
};
