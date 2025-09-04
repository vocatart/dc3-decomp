#pragma once
#include "obj/Object.h"
#include "utl/BinStream.h"
#include "utl/Symbol.h"

class FlowNode;

class FlowPtrBase {
public:
    int GetInitialState(Hmx::Object *);

protected:
    bool RefreshParamObject(void);
    Hmx::Object *GetObject(void);
    Hmx::Object *LoadObject(BinStream &);

    Symbol mObjName; // 0x0
    FlowNode *unk4; // 0x4
    int unk8; // 0x8
};

ObjectDir *FlowPtrGetLoadingDir(ObjectDir *);

template <typename T>
class FlowPtr : public FlowPtrBase {
public:
    FlowPtr(Hmx::Object *owner, T *ptr) : unkc(owner, ptr) {}
    FlowPtr(const FlowPtr &);
    ~FlowPtr();
    void operator=(T *);
    T *operator->();
    T *LoadFromMainOrDir(BinStream &);

private:
    T *Get();

    ObjPtr<T> unkc; // 0xc
};

template <typename T>
BinStream &operator<<(BinStream &, const FlowPtr<T> &);
