#pragma once
#include "types.h"
#include "os/Debug.h"
#include "rndobj/ShaderOptions.h"
#include "rndobj/Mat.h"
#include "rndobj/Mat_NG.h"

class RndShader {
public:
    enum MatFlagErrorType {
    };

    virtual bool CheckError(MatFlagErrorType) { return false; }

    static void Init();

protected:
    virtual void Select(RndMat *, ShaderType, bool) = 0;
    virtual u64 CalcShaderOpts(NgMat *, ShaderType, bool) = 0;

    void CheckForceCull(ShaderType);
    void Cache(ShaderType, ShaderOptions, RndMat *);
    bool RedundantState(const RndMat *, ShaderType, bool, bool, bool);

    static void ShaderWarn(const char *);
    static void WarnMatProp(const char *, NgMat *, class NgEnviron *, ShaderType);
    static bool MatShaderFlagsOK(RndMat *, ShaderType);
    static bool DisplayMatShaderFlagsError(RndMat *, ShaderType);

    static bool sCurrentSkinned;
    static bool sCurrentUseAO;
    static bool sMatShadersOK;
    static ModalCallbackFunc *mModalCallback;
    static ShaderType sCurrentShader;
};

class RndShaderSimple : public RndShader {
protected:
    virtual void Select(RndMat *, ShaderType, bool);
    virtual u64 CalcShaderOpts(NgMat *, ShaderType, bool);
};
