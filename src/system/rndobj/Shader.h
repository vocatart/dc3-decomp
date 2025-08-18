#pragma once
#include "types.h"

class RndShader {
public:
    enum MatFlagErrorType {
    };

    virtual bool CheckError(MatFlagErrorType) { return false; }

protected:
    // virtual void Select(RndMat *, ShaderType, bool) = 0;
    // virtual u64 CalcShaderOpts(NgMat *, ShaderType, bool) = 0;
};
