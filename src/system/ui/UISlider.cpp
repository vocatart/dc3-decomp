#include "ui/UISlider.h"
#include "utl/BinStream.h"

void UISlider::OldResourcePreload(BinStream &bs) {
    char buf[256];
    bs.ReadString(buf, 256);
    unk50.SetName(buf, true);
}

UISlider::UISlider() : unk50(this), mCurrent(0), mNumSteps(10), mVertical(0) {}
