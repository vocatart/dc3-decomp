#pragma once
#include "types.h"
#include "utl/Str.h"
#include <vector>

enum ShaderType {
    kShaderTypeBloom = 0,
    kShaderTypeBlur = 1,
    kShaderTypeDepthVolume = 2,
    kShaderTypeDownsample = 3,
    kShaderTypeDownsample4x = 4,
    kShaderTypeDownsampleDepth = 5,
    kShaderTypeDrawRect = 6,
    kShaderTypeError = 7,
    kShaderTypeFur = 8,
    kShaderTypeLineNoz = 9,
    kShaderTypeLine = 10,
    kShaderTypeMovie = 11,
    kShaderTypeMultimesh = 12,
    kShaderTypeMultimeshBB = 13,
    kShaderTypeParticles = 14,
    kShaderTypePostProcError = 15,
    kShaderTypePostProcess = 16,
    kShaderTypeShadowmap = 17,
    kShaderTypeStandard = 18,
    // where 19
    kShaderTypeSyncTrack = 20,
    // where 21
    kShaderTypeUnwrapUV = 22,
    kShaderTypeVelocityCamera = 23,
    kShaderTypeVelocityObject = 24,
    kShaderTypePlayerDepthVis = 25,
    kShaderTypePlayerDepthShell = 26,
    kShaderTypeBloomGlare = 27,
    kShaderTypePlayerDepthShell2 = 28,
    kShaderTypeDepthBuffer3D = 29,
    kShaderTypeYUVtoRGB = 30,
    kShaderTypeYUVtoBlackAndWhite = 31,
    kShaderTypePlayerGreenScreen = 32,
    kShaderTypePlayerDepthGreenScreen = 33,
    kShaderTypeCrewPhoto = 34,
    kShaderTypeTwirl = 35,
    kShaderTypeKillAlpha = 36,
    kShaderTypeAllWhite = 37,
    kMaxShaderTypes = 38
};

struct ShaderMacro {
    const char *unk0;
    const char *unk4;
};

struct ShaderOptions {
    void GenerateMacros(ShaderType, std::vector<ShaderMacro> &) const;

    u64 unk;
};

void InitShaderOptions();
const char *ShaderTypeName(ShaderType);
ShaderType ShaderTypeFromName(const char *);
const char *ShaderSourcePath(const char *);
const char *ShaderCachedPath(const char *, u64, bool);
bool IsPostProcShaderType(ShaderType);
void ShaderMakeOptionsString(ShaderType, const ShaderOptions &, String &);
