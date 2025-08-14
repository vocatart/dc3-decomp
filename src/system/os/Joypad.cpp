#include "os/Joypad.h"
#include "obj/Data.h"
#include "obj/DataFunc.h"
#include "obj/Msg.h"
#include "os/Debug.h"
#include "obj/Object.h"
#include "os/System.h"

namespace {

    DataArray *gControllersCfg; // 0x0
    DataArray *gButtonMeanings; // 0x4
    // static unsigned int gPadsToKeepAlive; // 0x8
    // static unsigned int gPadsToKeepAliveNext; // 0xc
    // static int gKeepAliveCountdown; // 0x10
    int gKeepaliveThresholdMs = -1;
    bool gExportMsgs = true;
    bool gJoypadLibInitialized;
    JoypadData gJoypadData[4];
    // static unsigned int gHolmesPressed; // 0x288
    bool gJoypadDisabled[4]; // 0x28c
    Hmx::Object *gJoypadMsgSource; // 0x290
}

JoypadData::JoypadData()
    : mButtons(0), mNewPressed(0), mNewReleased(0), mUser(nullptr), mConnected(false),
      mVibrateEnabled(true), unk4a(0), unk4b(0), unk4c(0), mNumAnalogSticks(0),
      mTranslateSticks(false), mIgnoreButtonMask(0), mGreenCymbalMask(0),
      mYellowCymbalMask(0), mBlueCymbalMask(0), mSecondaryPedalMask(0), mCymbalMask(0),
      mIsDrum(false), mType(kJoypadNone), mControllerType(), mDistFromRest(0),
      mHasGreenCymbal(false), mHasYellowCymbal(false), mHasBlueCymbal(false),
      mHasSecondaryPedal(false), unk84(0), unk94(0), unk9c(0), unka0(0), unka4(0),
      unka8(0), unkac(0), unkc0(0), unkd8(0) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mSticks[i][j] = 0;
        }
    }
    for (int i = 0; i < 2; i++) {
        mTriggers[i] = 0;
    }
}

float JoypadData::GetAxis(Symbol axis) const {
    static Symbol lx("LX");
    static Symbol ly("LY");
    static Symbol rx("RX");
    static Symbol ry("RY");
    static Symbol tl("TL");
    static Symbol tr("TR");
    static Symbol sx("SX");
    static Symbol sy("SY");
    static Symbol sz("SZ");
    if (axis == lx)
        return GetLX();
    else if (axis == ly)
        return GetLY();
    else if (axis == rx)
        return GetRX();
    else if (axis == ry)
        return GetRY();
    else if (axis == tl)
        return GetLT();
    else if (axis == tr)
        return GetRT();
    else if (axis == sx)
        return GetSX();
    else if (axis == sy)
        return GetSY();
    else if (axis == sz)
        return GetSZ();
    else
        MILO_FAIL("Bad axis %s in JoypadData::GetAxis()\n");
    return 0.0f;
}

int JoypadData::FloatToBucket(float f) const {
    if (f < 0.11f)
        return 0;
    if (f < 0.31f)
        return 6;
    if (f < 0.46f)
        return 5;
    if (f < 0.61f)
        return 4;
    if (f < 0.77f)
        return 3;
    if (f < 0.89f)
        return 2;
    return 1;
}

int JoypadData::GetVelocityBucket(Symbol axis) const {
    float ax = GetAxis(axis);
    if (ax < 0.0f)
        ax += 1.0f;
    return FloatToBucket(ax);
}

void JoypadTerminateCommon() {
    gJoypadLibInitialized = false;
    RELEASE(gJoypadMsgSource);
    // RELEASE(gKeyboardExporter);
}

void JoypadSubscribe(Hmx::Object *obj) {
    if (gJoypadMsgSource) {
        gJoypadMsgSource->AddSink(obj, Symbol());
    }
}

void JoypadUnsubscribe(Hmx::Object *obj) {
    if (gJoypadMsgSource) {
        gJoypadMsgSource->RemoveSink(obj, Symbol());
    }
}

JoypadData *JoypadGetPadData(int pad_num) {
    MILO_ASSERT(0 <= pad_num && pad_num < kNumJoypads, 0x47B);
    return &gJoypadData[pad_num];
}

bool JoypadVibrate(int pad) { return JoypadGetPadData(pad)->mVibrateEnabled; }

bool JoypadIsConnectedPadNum(int padNum) {
    if (padNum == -1)
        return false;
    else
        return gJoypadData[padNum].mConnected;
}

namespace {
    void Export(const Message &msg) {
        if (gExportMsgs) {
            gJoypadMsgSource->Handle(msg, false);
        }
    }

    DataNode OnJoypadSetVibrate(DataArray *arr) {
        JoypadSetVibrate(arr->Int(1), arr->Int(2) != 0);
        return 1;
    }

    DataNode OnJoypadVibrate(DataArray *arr) { return JoypadVibrate(arr->Int(1)); }

    DataNode OnJoypadControllerTypePadNum(DataArray *arr) {
        return JoypadControllerTypePadNum(arr->Int(1));
    }

    DataNode OnJoypadIsConnectedPadNum(DataArray *arr) {
        return JoypadIsConnectedPadNum(arr->Int(1));
    }

    DataNode OnJoypadIsButtonDownPadNum(DataArray *arr) {
        int pad = arr->Int(1);
        MILO_ASSERT((0) <= (pad) && (pad) < (kNumJoypads), 0x7F);
        int ret = gJoypadData[pad].mButtons & 1 << arr->Int(2);
        return ret != 0;
    }

    DataNode OnJoypadStageKitRaw(DataArray *arr) {
        arr->Int(2);
        arr->Int(1);
        return 1;
    }

    DataNode OnJoypadIsCalbertGuitar(DataArray *arr) {
        return JoypadIsCalbertGuitar(arr->Int(1)) != 0;
    }

    DataNode DataJoypadReset(DataArray *) {
        JoypadReset();
        return 0;
    }
}

void JoypadInitCommon(DataArray *joypad_config) {
    gJoypadMsgSource = Hmx::Object::New<Hmx::Object>();

    float thresh;
    joypad_config->FindData("threshold", thresh, true);
    joypad_config->FindData("keepalive_ms", gKeepaliveThresholdMs, true);
    for (int i = 0; i < 4; i++) {
        gJoypadData[i].mDistFromRest = thresh;
        gJoypadDisabled[i] = false;
    }

    DataArray *ignores = joypad_config->FindArray("ignore");
    for (int i = 1; i < ignores->Size(); i++) {
        int nodeInt = ignores->Int(i);
        if (nodeInt >= 0 && nodeInt < 4) {
            gJoypadDisabled[nodeInt] = true;
        }
    }
    gControllersCfg = joypad_config->FindArray("controllers");
    gButtonMeanings = joypad_config->FindArray("button_meanings");
    DataRegisterFunc("joypad_reset", DataJoypadReset);
    DataRegisterFunc("joypad_vibrate", OnJoypadVibrate);
    DataRegisterFunc("joypad_set_vibrate", OnJoypadSetVibrate);
    DataRegisterFunc("joypad_controller_type_padnum", OnJoypadControllerTypePadNum);
    DataRegisterFunc("joypad_is_connected_padnum", OnJoypadIsConnectedPadNum);
    DataRegisterFunc("joypad_is_button_down", OnJoypadIsButtonDownPadNum);
    DataRegisterFunc("joypad_stage_kit_raw", OnJoypadStageKitRaw);
    DataRegisterFunc("joypad_is_calbert_guitar", OnJoypadIsCalbertGuitar);
    gJoypadLibInitialized = true;
}
