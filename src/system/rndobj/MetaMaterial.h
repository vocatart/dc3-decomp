#pragma once
#include "rndobj/BaseMaterial.h"

// size 0x204
class MetaMaterial : public BaseMaterial {
public:
    virtual ~MetaMaterial();
    OBJ_CLASSNAME(MetaMaterial);
    OBJ_SET_TYPE(MetaMaterial);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

protected:
    MetaMaterial();
    int CalcApproxNumShaders();
    void SetEditAction(MatProp, MatPropEditAction);

    std::vector<MatPropEditAction> unk1f8;
};
