#pragma once
#include "math/Color.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "rndobj/BaseMaterial.h"
#include "rndobj/MetaMaterial.h"
#include "rndobj/Tex.h"

struct MatShaderOptions {
    MatShaderOptions() : unk0(0x12), unk4(0) {}
    unsigned int unk0;
    bool unk4;
};

class RndMat : public BaseMaterial {
public:
    enum PropDisplay {
        kPropDisplayHidden = 0,
        kPropDisplayReadOnly = 1
    };
    enum ColorModFlags {
        kColorModNone = 0,
        kColorModAlphaPack = 1,
        kColorModAlphaUnpackModulate = 2,
        kColorModModulate = 3,
        kColorModNum = 3
    };
    virtual ~RndMat();
    OBJ_CLASSNAME(Mat);
    OBJ_SET_TYPE(Mat);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    NEW_OBJ(RndMat);

    float GetRefractStrength() { return mRefractStrength; }
    RndTex *GetRefractNormalMap() { return mRefractNormalMap; }
    void SetColorMod(const Hmx::Color &, int);
    void SetSpecularMap(RndTex *);
    void SetMetaMat(MetaMaterial *, bool);
    MetaMaterial *CreateMetaMaterial(bool);

    static void UpdateAllMatPropertiesFromMetaMat(ObjectDir *);

protected:
    RndMat();

    bool IsEditable(Symbol);
    MatPropEditAction GetMetaMatPropAction(Symbol);
    bool OnGetPropertyDisplay(PropDisplay, Symbol);
    void UpdatePropertiesFromMetaMat();

    DataNode OnGetMetaMaterials(const DataArray *);
    DataNode OnGetMetaMaterialsDir(const DataArray *);

    static ObjectDir *sMetaMaterials;
    static ObjectDir *LoadMetaMaterials();

    ObjPtr<MetaMaterial> mMetaMaterial; // 0x1f8
    int unk20c;
    std::vector<Hmx::Color> mColorMod; // 0x210
    MatShaderOptions mShaderOptions; // 0x21c
    bool mToggleDisplayAllProps; // 0x224
    bool unk225;
    bool unk226;
    int mDirty; // 0x228
};

RndMat *LookupOrCreateMat(const char *, ObjectDir *);
