#pragma once
#include "obj/Object.h"
#include "rndobj/Mat.h"

class NgMat : public RndMat {
public:
    NgMat();
    virtual ~NgMat() {
        if (sCurrent == this)
            sCurrent = nullptr;
    }
    OBJ_CLASSNAME(Mat);
    OBJ_SET_TYPE(Mat);

    bool AllowFog() const;
    bool AllowHDR() const;
    void SetupShader(bool, bool);

    NEW_OBJ(NgMat);

protected:
    static NgMat *sCurrent;

    void SetupAmbient();
    void SetBasicState();
    void RefreshState();
    void SetRegularShaderConst(bool);
};
