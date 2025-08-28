#include "synth/OggMap.h"
#include "math/Utl.h"
#include "os/Debug.h"

int OggMap::GetSongLengthSamples() { return mGran * mLookup.size(); }

void OggMap::GetSeekPos(int sampTarget, int &seekPos, int &actSamp) {
    int i14 = sampTarget / mGran;
    int i18 = mLookup.size() - 1;
    ClampEq(i14, 0, i18);
    seekPos = mLookup[i14].first;
    actSamp = mLookup[i14].second;
}

OggMap::~OggMap() { mLookup.clear(); }

OggMap::OggMap() : mGran(1000), mLookup() {
    mLookup.push_back(std::pair<int, int>(0, 0));
}

void OggMap::Read(BinStream &bs) {
    int version;
    bs >> version;
    if (version < 0xb)
        MILO_FAIL("Incorrect oggmap version.");
    bs >> mGran >> mLookup;
}
