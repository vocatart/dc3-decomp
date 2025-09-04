#pragma once
#include "flow/FlowPtr.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "utl/BinStream.h"

class FlowMathOp {
public:
    void Save(BinStream &);

protected:
    float unk0;
    int unk4;
    DataNode unk8;
    DataNode unk10;
    FlowPtr<Hmx::Object> unk18;
};
