#include "ui/UILabel.h"
#include "utl/BinStream.h"

void UILabel::Load(BinStream &bs) {
    PreLoad(bs);
    PostLoad(bs);
}

UILabel::UILabel() : unk122(1), unk124(this) {
    unk124.resize(1);
    unk120 = false;
    unk121 = false;
}
