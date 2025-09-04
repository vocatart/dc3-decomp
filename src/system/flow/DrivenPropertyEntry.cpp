#include "flow/DrivenPropertyEntry.h"
#include "FlowMathOp.h"
#include "obj/Object.h"
#include "utl/BinStream.h"

DrivenPropertyEntry::DrivenPropertyEntry(Hmx::Object *owner) : mMathOps(owner) {
    static Symbol none("none");
    unk0 = none;
}

DrivenPropertyEntry::~DrivenPropertyEntry() { mMathOps.clear(); }

void DrivenPropertyEntry::Save(BinStream &bs) {
    bs << 0;
    bs << unk0;
    bs << mMathOps.size();
    for (ObjVector<FlowMathOp>::iterator it = mMathOps.begin(); it != mMathOps.end();
         ++it) {
        it->Save(bs);
    }
}
