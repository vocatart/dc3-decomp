#include "rndobj/Shader.h"
#include "Mat_NG.h"
#include "os/Debug.h"
#include "rndobj/Mat.h"
#include "rndobj/ShaderOptions.h"
#include "utl/Loader.h"
#include "utl/Str.h"
#include <set>

std::set<unsigned int> sWarnings;
RndShaderSimple gShaderSimple;

unsigned int StrHash(const char *str);

void RndShader::ShaderWarn(const char *msg) {
    unsigned int hash = StrHash(msg);
    if (sWarnings.end() == sWarnings.find(hash)) {
        MILO_NOTIFY(msg);
        sWarnings.insert(hash);
    }
    if (TheLoadMgr.EditMode()) {
        Debug::ModalType ty = (Debug::ModalType)1;
        if (mModalCallback) {
            StackString<1024> str(msg);
            (*mModalCallback)(ty, str, true);
        }
    }
}

void RndShader::WarnMatProp(const char *prop, NgMat *mat, NgEnviron *env, ShaderType s) {
    ShaderWarn(MakeString(
        "[%s] must have %s.  (%s, %s)",
        PathName(mat),
        prop,
        /*PathName(env),*/
        ShaderTypeName(s)
    ));
    sMatShadersOK = false;
}

void RndShaderSimple::Select(RndMat *mat, ShaderType s, bool b) {}
