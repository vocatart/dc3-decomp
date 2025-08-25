#include "ui/UIListWidget.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "ui/UIColor.h"
#include "ui/UIComponent.h"
#include "math/Vec.h"
#include "utl/BinStream.h"

float UIListWidget::DrawOrder() const { return mDrawOrder; }

void UIListWidget::CalcXfm(const Transform &tfin, const Vector3 &vin, Transform &out) {
    out.v.x += vin.x;
    out.v.y += vin.y;
    out.v.z += vin.z;
    Multiply(out, tfin, out);
}

void UIListWidget::SetParentList(UIList *list) { mParentList = list; }

UIColor *UIListWidget::DisplayColor(
    UIListWidgetState element_state, UIComponent::State list_state
) const {
    MILO_ASSERT((0) <= (element_state) && (element_state) < (kNumUIListWidgetStates), 0x64);
    MILO_ASSERT((0) <= (list_state) && (list_state) < (UIComponent::kNumStates), 0x65);
    UIColor *color = mColors[element_state][list_state];
    if (color)
        return color;
    else if (mDefaultColor)
        return mDefaultColor;
    else
        return nullptr;
}

void UIListWidget::Save(BinStream &bs) {
    bs << 2;
    SAVE_SUPERCLASS(Hmx::Object)
    bs << mDrawOrder << mDefaultColor << mWidgetDrawType;
    bs << mDisabledAlphaScale;
    for (int i = 0; i < kNumUIListWidgetStates; i++) {
        for (int j = 0; j < UIComponent::kNumStates; j++) {
            bs << mColors[i][j];
        }
    }
}

UIListWidget::UIListWidget()
    : mDrawOrder(0), mDisabledAlphaScale(1), mDefaultColor(this),
      mWidgetDrawType(kUIListWidgetDrawAlways), mParentList(nullptr) {
    for (int i = 0; i < kNumUIListWidgetStates; i++) {
        std::vector<ObjPtr<UIColor> > vec;
        for (int j = 0; j < UIComponent::kNumStates; j++) {
            vec.push_back(ObjPtr<UIColor>(this));
        }
        mColors.push_back(vec);
    }
}
