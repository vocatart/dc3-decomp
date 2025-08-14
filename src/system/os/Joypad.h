#pragma once
#include "obj/Object.h"
#include "utl/Symbol.h"

enum {
    kNumJoypads = 4,
    kNumPressureButtons = 8,
};

class JoypadData {
private:
    unsigned int mButtons; // 0x0
    unsigned int mNewPressed; // 0x4
    unsigned int mNewReleased; // 0x8
    float mSticks[2][2]; // 0xC = LX; 0x10 = LY; 0x14 = RX; 0x18 = RY
    float mTriggers[2]; // 0x1C = LT; 0x20 = RT
    float mSensors[3]; // 0x24 = SX; 0x28 = SY; 0x2C = SZ
    float mPressures[8]; // 0x30 - 0x50

    int FloatToBucket(float) const;

public:
    JoypadData();
    float GetAxis(Symbol) const;
    int GetVelocityBucket(Symbol) const;

    float GetLX() const { return mSticks[0][0]; }
    float GetLY() const { return mSticks[0][1]; }
    float GetRX() const { return mSticks[1][0]; }
    float GetRY() const { return mSticks[1][1]; }
    float GetLT() const { return mTriggers[0]; }
    float GetRT() const { return mTriggers[1]; }
    float GetSX() const { return mSensors[0]; }
    float GetSY() const { return mSensors[1]; }
    float GetSZ() const { return mSensors[2]; }
};

extern "C" {
void JoypadInitCommon(DataArray *);
void JoypadTerminateCommon();
void JoypadPollCommon();
}

void JoypadReset();
JoypadData *JoypadGetPadData(int);
Symbol JoypadControllerTypePadNum(int);

void JoypadSubscribe(Hmx::Object *);
void JoypadUnsubscribe(Hmx::Object *);
void JoypadSetVibrate(int, bool);
