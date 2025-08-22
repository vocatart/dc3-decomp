#pragma once
#include "obj/Object.h"
#include "rndobj/Mat.h"

class RndFontBase : public Hmx::Object {
public:
    class KernInfo {
    public:
        int unk0, unk4;
    };

    OBJ_CLASSNAME(FontBase);
    OBJ_SET_TYPE(FontBase);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual float CharWidth(unsigned short) const { return 0; }
    virtual bool CharAdvance(unsigned short, unsigned short, float &) const {
        return false;
    }
    virtual float CharAdvance(unsigned short) const { return 0; }
    virtual float Kerning(unsigned short, unsigned short) const;
    virtual bool CharDefined(unsigned short) const;
    virtual float AspectRatio() const { return 0; }
    virtual RndMat *Mat() const { return nullptr; }
    virtual const RndFontBase *DataOwner() const { return this; }
    virtual float FontUnit() const { return 0; }
    virtual float FontUnitInverse() const { return 1.0f / FontUnit(); }
    virtual void Print() const {}
    virtual bool BitmapFont() const { return true; }

    void SetBaseKerning(float);
    void SetKerning(const std::vector<KernInfo> &);
    void GetKerning(std::vector<KernInfo> &) const;

protected:
    RndFontBase();
    virtual bool HasChar(unsigned short) const;

    String GetASCIIChars() const;
    void SetASCIIChars(String);

    std::vector<unsigned short> mChars; // 0x2c
    bool unk38; // 0x38
    float mBaseKerning; // 0x3c
    class KerningTable *mKerningTable; // 0x40
};
