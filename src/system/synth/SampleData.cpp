#include "synth/SampleData.h"

int SampleData::NumMarkers() const { return mMarkers.size(); }

SampleData::~SampleData() { Dealloc(); }

void SampleData::Reset() {
    Dealloc();
    mFormat = kPCM;
    mSizeBytes = 0;
    mSampleRate = 0;
    mNumSamples = 0;
    mNumChannels = 1;
    mMarkers.clear();
}

SampleData::SampleData() : mData(0), mMarkers() { Reset(); }
