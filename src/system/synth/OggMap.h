#pragma once
#include "utl/BinStream.h"
#include <vector>

class OggMap {
public:
    OggMap();
    virtual ~OggMap();

    void Read(BinStream &);
    void GetSeekPos(int, int &, int &);
    int GetSongLengthSamples();

private:
    int mGran; // 0x4
    std::vector<std::pair<int, int> > mLookup; // 0x8
};
