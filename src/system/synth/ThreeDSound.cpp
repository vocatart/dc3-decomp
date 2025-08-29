#include "synth/ThreeDSound.h"
#include "obj/Object.h"
#include "rndobj/Trans.h"
#include "rndobj/Utl.h"

void ThreeDSound::SaveWorldXfm() { unk1cc = WorldXfm(); }

bool ThreeDSound::HasMoved() { return WorldXfm() != unk1cc; }

ThreeDSound::ThreeDSound()
    : unk194(0), unk195(0), unk198(0), unk19c(0), unk1a0(0), unk1a4(0), unk1a8(0),
      unk1ac(0), unk1b0(2), unk1b4(10), unk1b8(100), unk1bc(1), unk1bd(1), unk1c0(0),
      unk1c4(10), unk20c(100), unk210(0), unk214(1), unk218(0) {
    Fader *fader = static_cast<Fader *>(Fader::NewObject());
    unk1c8 = fader;
    mFaders.Add(unk1c8);
    CalculateFaderVolume();
    Vector3 v(unk1b4, unk1b8, 1);
    SetLocalScale(this, v);
}

BEGIN_PROPSYNCS(ThreeDSound)
    SYNC_PROP_SET(enable_doppler, unk1bc, bool doppler = _val.Int();
                  unk1c8->SetTranspose(0);
                  unk1bc = doppler)
    SYNC_PROP_SET(enable_pan, unk1bd, EnablePan(_val.Int()))
    SYNC_PROP_SET(falloff_type, unk1ac, unk1ac = _val.Int(); CalculateFaderVolume();)
    SYNC_PROP_SET(falloff_parameter, unk1a8, unk1a8 = _val.Int(); CalculateFaderVolume();)

END_PROPSYNCS

BEGIN_HANDLERS(ThreeDSound)
    HANDLE_SUPERCLASS(Sound)
    HANDLE_SUPERCLASS(RndTransformable)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS
