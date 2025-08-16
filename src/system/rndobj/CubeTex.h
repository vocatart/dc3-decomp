#pragma once
#include "obj/Object.h"
#include "utl/BinStream.h"
#include "utl/FilePath.h"
#include "rndobj/Bitmap.h"
#include "utl/MemMgr.h"

class RndCubeTex : public Hmx::Object {
public:
    enum CubeFace {
        kCubeFaceRight,
        kCubeFaceLeft,
        kCubeFaceTop,
        kCubeFaceBottom,
        kCubeFaceFront,
        kCubeFaceBack,
        kNumCubeFaces
    };

    class CubeTexProperties {
    public:
        CubeTexProperties() { Init(); }
        CubeTexProperties &operator=(const CubeTexProperties &t) {
            mWidth = t.mWidth;
            mHeight = t.mHeight;
            mBpp = t.mBpp;
            mNumMips = t.mNumMips;
            mOrder = t.mOrder;
            return *this;
        }
        void Init() { mOrder = mNumMips = mBpp = mHeight = mWidth = 0; }
        void Set(const RndBitmap &);
        void Load(BinStream &);
        void Save(BinStream &);

        int mWidth, mHeight, mBpp, mNumMips, mOrder;
    };

    virtual ~RndCubeTex() {}
    OBJ_CLASSNAME(CubeTex);
    OBJ_SET_TYPE(CubeTex);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    virtual void Select(int) {}
    virtual void Reset() {}
    virtual void Sync() {}

    OBJ_MEM_OVERLOAD(0x17);

    void SetBitmap(CubeFace, const FilePath &, bool);
    void UpdateFace(CubeFace);

private:
    bool LoadBitmap(const FilePath &, RndBitmap &);
    bool ValidateBitmapProperties(std::vector<CubeFace> &);
    void Update();

protected:
    RndCubeTex();

    CubeTexProperties props; // 0x2c
    CubeTexProperties moreprops[kNumCubeFaces]; // 0x40
    FilePath mFile[kNumCubeFaces]; // 0xb8
    RndBitmap mBitmap[kNumCubeFaces]; // 0xe8
};
