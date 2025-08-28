#include "synth/MoggClip.h"
#include "obj/Data.h"

void MoggClip::Pause(bool pause) { mPlaying = !pause; }

bool IsLoadingMusicMogg(const char *mogg) {
    static Symbol is_loading_music_mogg("is_loading_music_mogg");
    static DataArrayPtr func(new DataArray(2));
    func->Node(0) = is_loading_music_mogg;
    func->Node(1) = mogg;
    DataNode exec = func->Execute(false);
    return exec.Int();
}

bool IsUselessMogg(const char *mogg) {
    static Symbol is_useless_mogg_load("is_useless_mogg_load");
    static DataArrayPtr func(new DataArray(2));
    func->Node(0) = is_useless_mogg_load;
    func->Node(1) = mogg;
    DataNode exec = func->Execute(false);
    return exec.Int();
}

MoggClip::MoggClip() : unk78(this) {}
