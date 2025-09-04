#include "gesture/CameraInput.h"

void CameraInput::PollTracking() {
    unk8 = PollNewFrame();
    if (unk8) {
        memcpy(&unkc, unk8, sizeof(unkc));
    }
}

CameraInput::CameraInput() : unk8(nullptr) { memset(&unkc, 0, sizeof(unkc)); }

CameraInput::~CameraInput() {}
