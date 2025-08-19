#include "ShaderOptions.h"
#include "os/Debug.h"
#include "rndobj/ShaderOptions.h"
#include "os/File.h"
#include "os/System.h"
#include "utl/Loader.h"
#include "utl/Symbol.h"

Symbol sShaderTypes[kMaxShaderTypes];

void InitShaderOptions() {
    sShaderTypes[kShaderTypeBloom] = "bloom";
    sShaderTypes[kShaderTypeBloomGlare] = "bloom_glare";
    sShaderTypes[kShaderTypeBlur] = "blur";
    sShaderTypes[kShaderTypeDepthVolume] = "depthvolume";
    sShaderTypes[kShaderTypeDownsampleDepth] = "downsample_depth";
    sShaderTypes[kShaderTypeDownsample] = "downsample";
    sShaderTypes[kShaderTypeDownsample4x] = "downsample_4x";
    sShaderTypes[kShaderTypeDrawRect] = "drawrect";
    sShaderTypes[kShaderTypeError] = "error";
    sShaderTypes[kShaderTypeFur] = "fur";
    sShaderTypes[kShaderTypeLineNoz] = "line_noz";
    sShaderTypes[kShaderTypeLine] = "line";
    sShaderTypes[kShaderTypeMovie] = "movie";
    sShaderTypes[kShaderTypeMultimeshBB] = "multimesh_bb";
    sShaderTypes[kShaderTypeMultimesh] = "multimesh";
    sShaderTypes[kShaderTypeParticles] = "particles";
    sShaderTypes[kShaderTypePostProcError] = "postproc_error";
    sShaderTypes[kShaderTypePostProcess] = "postprocess";
    sShaderTypes[kShaderTypeShadowmap] = "shadowmap";
    sShaderTypes[kShaderTypeStandard] = "standard";
    sShaderTypes[kShaderTypeSyncTrack] = "sync_track";
    sShaderTypes[kShaderTypeUnwrapUV] = "unwrapuv";
    sShaderTypes[kShaderTypeVelocityCamera] = "velocity_camera";
    sShaderTypes[kShaderTypeVelocityObject] = "velocity_object";
    sShaderTypes[kShaderTypePlayerDepthVis] = "playerdepth_vis";
    sShaderTypes[kShaderTypePlayerDepthShell] = "playerdepth_shell";
    sShaderTypes[kShaderTypePlayerDepthShell2] = "playerdepth_shell2";
    sShaderTypes[kShaderTypeDepthBuffer3D] = "depthbuffer_3d";
    sShaderTypes[kShaderTypeYUVtoRGB] = "yuv_to_rgb";
    sShaderTypes[kShaderTypeYUVtoBlackAndWhite] = "yuv_to_black_and_white";
    sShaderTypes[kShaderTypePlayerGreenScreen] = "player_greenscreen";
    sShaderTypes[kShaderTypePlayerDepthGreenScreen] = "player_depthgreenscreen";
    sShaderTypes[kShaderTypeCrewPhoto] = "crew_photo";
    sShaderTypes[kShaderTypeTwirl] = "twirl";
    sShaderTypes[kShaderTypeKillAlpha] = "killalpha";
    sShaderTypes[kShaderTypeAllWhite] = "allwhite";
}

const char *ShaderTypeName(ShaderType shader) {
    MILO_ASSERT(shader >= ShaderType(0) && shader < kMaxShaderTypes, 0x41);
    return sShaderTypes[shader].Str();
}

const char *sShaderErrors[] = { "pulse" };

ShaderType ShaderTypeFromName(const char *name) {
    for (int i = 0; i < kMaxShaderTypes; i++) {
        if (streq(name, sShaderTypes[i].Str())) {
            return (ShaderType)i;
        }
    }
    for (int i = 0; i < DIM(sShaderErrors); i++) {
        if (streq(name, sShaderErrors[i])) {
            return kShaderTypeError;
        }
    }
    MILO_FAIL("Shader type name %s not found\n", name);
    return (ShaderType)-1;
}

const char *ShaderSourcePath(const char *file) {
    if (file) {
        return FileMakePath(
            FileSystemRoot(), MakeString("shaders/%s.fx", FileGetBase(file))
        );
    } else {
        return FileMakePath(FileSystemRoot(), "shaders");
    }
}

const char *ShaderCachedPath(const char *file, u64 i2, bool b3) {
    Platform plat = TheLoadMgr.GetPlatform();
    return MakeString(
        "%s/gen/%s_%llx_%s.%s_%s",
        FileGetPath(file),
        FileGetBase(file),
        i2,
        b3 ? "ps" : "vs",
        FileGetExt(file),
        PlatformSymbol(plat)
    );
}

bool IsPostProcShaderType(ShaderType s) {
    switch (s) {
    case kShaderTypeBloom:
    case kShaderTypeBlur:
    case kShaderTypeDepthVolume:
    case kShaderTypeDownsample:
    case kShaderTypeDownsample4x:
    case kShaderTypeDownsampleDepth:
    case kShaderTypeDrawRect:
        return false;
    case kShaderTypeError:
        return true;
    case kShaderTypeFur:
    case kShaderTypeLineNoz:
    case kShaderTypeLine:
    case kShaderTypeMovie:
        return false;
    case kShaderTypeMultimesh:
    case kShaderTypeMultimeshBB:
    case kShaderTypeParticles:
        return true;
    case kShaderTypePostProcError:
    case kShaderTypePostProcess:
    case kShaderTypeShadowmap:
        return false;
    case kShaderTypeStandard:
    case (ShaderType)19:
    case kShaderTypeSyncTrack:
    case (ShaderType)21:
        return true;
    case kShaderTypeUnwrapUV:
    case kShaderTypeVelocityCamera:
    case kShaderTypeVelocityObject:
    case kShaderTypePlayerDepthVis:
    case kShaderTypePlayerDepthShell:
    case kShaderTypeBloomGlare:
    case kShaderTypePlayerDepthShell2:
    case kShaderTypeDepthBuffer3D:
    case kShaderTypeYUVtoRGB:
    case kShaderTypeYUVtoBlackAndWhite:
    case kShaderTypePlayerGreenScreen:
    case kShaderTypePlayerDepthGreenScreen:
    case kShaderTypeCrewPhoto:
    case kShaderTypeTwirl:
    case kShaderTypeKillAlpha:
        return false;
    case kShaderTypeAllWhite:
        return true;
    default:
        MILO_FAIL("unknown shader type %s", ShaderTypeName(s));
        return false;
    }
}
