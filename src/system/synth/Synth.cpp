#include "synth/Synth.h"
#include "obj/Dir.h"
#include "os/System.h"
#include "synth/ADSR.h"

namespace {
    struct DebugGraph {
        DebugGraph(const Hmx::Color &c) {
            unk0.resize(200);
            unk8 = 0;
            unkc = c;
        }

        std::vector<float> unk0;
        int unk8;
        Hmx::Color unkc;
    };

    std::vector<DebugGraph> gDebugGraphs;
}

Synth *TheSynth;

Synth::Synth()
    : mTrackLevels(false), mMuted(false), mMicClientMapper(nullptr), unk98(0), unk9c(0),
      mADSR(nullptr) {
    SetName("synth", ObjectDir::Main());
    DataArray *cfg = SystemConfig("synth");
    cfg->FindData("mics", mNumMics, true);
    cfg->FindData("track_levels", mTrackLevels, false);
    mMidiSynth = new MidiSynth();
    gDebugGraphs.push_back(DebugGraph(Hmx::Color(1, 0, 0)));
    gDebugGraphs.push_back(DebugGraph(Hmx::Color(0, 1, 0)));
    gDebugGraphs.push_back(DebugGraph(Hmx::Color(1, 1, 0)));
    gDebugGraphs.push_back(DebugGraph(Hmx::Color(1, 1, 1)));
    mMicClientMapper = new MicClientMapper();
    MILO_ASSERT(!TheSynth, 0xC0);
    mADSR = new ADSRImpl();
}
