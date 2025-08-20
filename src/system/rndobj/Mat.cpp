#include "rndobj/Mat.h"
#include "Utl.h"
#include "math/Color.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/DirLoader.h"
#include "obj/Object.h"
#include "obj/Utl.h"
#include "os/Debug.h"
#include "os/File.h"
#include "os/System.h"
#include "rndobj/BaseMaterial.h"
#include "rndobj/MetaMaterial.h"
#include "rndobj/Tex.h"
#include "utl/BinStream.h"
#include "utl/FilePath.h"
#include "utl/Loader.h"

const char *kAnonMetaMatPrefix = "{anon}";
const char *kMiloMetaMatPrefix = "{milo}";

MatPropEditAction RndMat::GetMetaMatPropAction(Symbol s) {
    String str(s);
    str += "_edit_action";
    const DataNode *node = mMetaMaterial->Property(str.c_str(), true);
    MILO_ASSERT(node, 0x2BE);
    return (MatPropEditAction)node->Int();
}

bool RndMat::OnGetPropertyDisplay(PropDisplay display, Symbol s) {
    MILO_ASSERT(display == kPropDisplayHidden || display == kPropDisplayReadOnly, 0x357);
    if (mMetaMaterial) {
        if (mToggleDisplayAllProps)
            return display == kPropDisplayHidden;
        else
            return true;
    } else {
        MatPropEditAction a = GetMetaMatPropAction(s);
        return a == 2 || (a != 0 && a != 1);
    }
    return false;
}

void RndMat::Save(BinStream &bs) {
    bs << 0x46;
    SAVE_SUPERCLASS(BaseMaterial)
    bs << mMetaMaterial;
}

void RndMat::SetColorMod(const Hmx::Color &color, int index) {
    MILO_ASSERT(index >= 0 && index < kColorModNum, 0x230);
    mColorMod[index] = color;
    mDirty |= 2;
}

DataNode RndMat::OnGetMetaMaterialsDir(const DataArray *) { return sMetaMaterials; }

RndMat *LookupOrCreateMat(const char *shader, ObjectDir *dir) {
    const char *fileStr = MakeString("%s.mat", FileGetBase(shader));
    RndMat *mat = dir->Find<RndMat>(fileStr, false);
    if (!mat) {
        mat = dir->Find<RndMat>(FileGetBase(shader), false);
        if (!mat) {
            bool old = TheLoadMgr.EditMode();
            TheLoadMgr.SetEditMode(true);
            mat = dir->New<RndMat>(fileStr);
            TheLoadMgr.SetEditMode(old);
        }
    }
    return mat;
}

void RndMat::SetSpecularMap(RndTex *tex) {
    if (tex && !mSpecularMap) {
        if (mSpecularRGB.Pack() == 0) {
            mSpecularRGB.Set(1, 1, 1, mSpecularRGB.alpha);
        }
    }
    mSpecularMap = tex;
    mDirty |= 2;
}

void RndMat::SetMetaMat(MetaMaterial *mat, bool b) {
    mMetaMaterial = mat;
    UpdatePropertiesFromMetaMat();
    unk225 = b;
}

namespace {
    void MergeMetaMaterials(ObjectDir *o1, ObjectDir *o2) {
        if (o2 && o1) {
            for (ObjDirItr<MetaMaterial> it(o2, true); it != nullptr; ++it) {
                MetaMaterial *mat = o1->Find<MetaMaterial>(it->Name(), false);
                if (!mat) {
                    mat = Hmx::Object::New<MetaMaterial>();
                    mat->SetName(it->Name(), o1);
                }
                CopyObject(it, mat, Hmx::Object::kCopyDeep, true);
            }
        }
    }
}

void RndMat::UpdateAllMatPropertiesFromMetaMat(ObjectDir *dir) {
    for (ObjDirItr<RndMat> it(dir, true); it != nullptr; ++it) {
        it->UpdatePropertiesFromMetaMat();
    }
}

ObjectDir *RndMat::LoadMetaMaterials() {
    const char *path = "";
    ObjectDir *dir = nullptr;
    DataArray *cfg = SystemConfig("objects", "Mat");
    if (cfg->FindData("metamaterial_path", path, false) && *path != '\0') {
        {
            FilePathTracker tracker(path);
            dir = DirLoader::LoadObjects("metamaterials.milo", nullptr, nullptr);
            MILO_ASSERT(dir, 0x99);
        }
        if (!strstr("system/run", FilePath::Root().c_str()) && TheLoadMgr.EditMode()) {
            ObjectDir *loadedDir = DirLoader::LoadObjects(
                "../../system/run/config/metamaterials.milo", nullptr, nullptr
            );
            MergeMetaMaterials(dir, loadedDir);
            delete loadedDir;
        }
    }
    return dir;
}

DataNode RndMat::OnGetMetaMaterials(const DataArray *a) {
    bool i2 = a->Int(2);
    int numMetaMats = 0;
    if (sMetaMaterials) {
        for (ObjDirItr<MetaMaterial> it(sMetaMaterials, true); it != nullptr; ++it) {
            numMetaMats++;
        }
    }
    DataArrayPtr ptr;
    ptr->Resize(numMetaMats + 1);
    ptr->Node(0) = NULL_OBJ;
    if (sMetaMaterials) {
        int idx = 1;
        for (ObjDirItr<MetaMaterial> it(sMetaMaterials, true); it != nullptr; ++it) {
            const char *name = it->Name();
            if (!strstr(name, kAnonMetaMatPrefix)
                && (i2 || !strstr(name, kMiloMetaMatPrefix))) {
                ptr->Node(idx++) = &*it;
            }
        }
        ptr->Resize(idx);
        ptr->SortNodes(0);
    }
    return ptr;
}

MetaMaterial *RndMat::CreateMetaMaterial(bool notify) {
    bool hasStr = false;
    String str(Name());
    if (str.empty()) {
        hasStr = true;
        str = kAnonMetaMatPrefix;
        str += ".";
        str += ClassExt("Mat");
    }
    int extlen = strlen(ClassExt("Mat"));
    if (strlen(str.c_str()) > extlen) {
        str.erase(str.length() - extlen);
    }
    str += ClassExt("MetaMaterial");
    MILO_ASSERT(sMetaMaterials, 0x30A);

    const char *nextname = NextName(str.c_str(), sMetaMaterials);
    MetaMaterial *mat = Hmx::Object::New<MetaMaterial>();
    mat->SetName(nextname, sMetaMaterials);
    mat->Copy(this, kCopyDeep);
    String strc0;
    std::list<Symbol> symList;
    ListProperties(symList, "Mat", 0, nullptr, false);
    for (std::list<Symbol>::iterator it = symList.begin(); it != symList.end(); ++it) {
        Symbol cur = *it;
        static Symbol metamaterial("metamaterial");
        if (cur != metamaterial) {
            bool b10 = false;
            const DataNode *node = Property(cur, true);
            if (node && node->Type() == kDataObject && node->GetObj()) {
                if (!strc0.empty()) {
                    strc0 += ", ";
                }
                strc0 += cur.Str();
                mat->SetProperty(cur, NULL_OBJ);
                b10 = true;
            }
            bool b9 = PropValDifferent(cur, nullptr);
            String stre0(cur);
            stre0 += "_edit_action";
            mat->SetProperty(stre0.c_str(), b10 ? 2 : b9 != 0);
        }
    }
    if (hasStr) {
        for (ObjDirItr<MetaMaterial> it(sMetaMaterials, true); it != nullptr; ++it) {
            if (mat != it && mat->IsEquivalent(it)) {
                delete mat;
                mat = it;
                break;
            }
        }
    }
    if (notify) {
        if (!strc0.empty()) {
            MILO_NOTIFY(
                "Some object properties were not copied to the MetaMaterial:%s",
                strc0.c_str()
            );
        }
    }
    return mat;
}

RndMat::RndMat()
    : mMetaMaterial(this), unk20c(0), mToggleDisplayAllProps(0), unk225(0), unk226(0),
      mDirty(3) {
    ResetColors(mColorMod, 3);
}

RndMat::~RndMat() {
    if (unk225 && mMetaMaterial) {
        if (mMetaMaterial->RefCount() == 1) {
            RELEASE(mMetaMaterial);
        }
    }
}

BEGIN_COPYS(RndMat)
    COPY_SUPERCLASS(BaseMaterial)
    CREATE_COPY(RndMat)
    BEGIN_COPYING_MEMBERS
        if (ty != kCopyFromMax) {
            COPY_MEMBER(mShaderOptions)
            COPY_MEMBER(unk20c)
            COPY_MEMBER(mColorMod)
            COPY_MEMBER(mMetaMaterial)
        }
        mDirty = 3;
        UpdatePropertiesFromMetaMat();
    END_COPYING_MEMBERS
END_COPYS

BEGIN_HANDLERS(RndMat)
    HANDLE(get_metamats_dir, OnGetMetaMaterialsDir)
    HANDLE(get_metamats, OnGetMetaMaterials)
    HANDLE_EXPR(prop_is_hidden, OnGetPropertyDisplay(kPropDisplayHidden, _msg->Sym(2)))
    HANDLE_EXPR(
        prop_is_read_only, OnGetPropertyDisplay(kPropDisplayReadOnly, _msg->Sym(2))
    )
    HANDLE_ACTION(
        toggle_display_all_props, mToggleDisplayAllProps = !mToggleDisplayAllProps
    )
    HANDLE_ACTION(create_metamat, CreateMetaMaterial(true))
    HANDLE_SUPERCLASS(BaseMaterial)
END_HANDLERS

bool RndMat::IsEditable(Symbol s) {
    if (mMetaMaterial && !unk226) {
        bool ret = mMetaMaterial->Property(s, true)->Int() == 2;
        if (!ret) {
            String str(s);
            int len = str.length();
            if (len > 12) {
                str = str.substr(0, len - 12);
            }
            MILO_NOTIFY_ONCE(
                "Unable to set property %s in Mat %s.  Not allowed by MetaMaterial %s.\n",
                str.c_str(),
                PathName(this),
                PathName(mMetaMaterial)
            );
        }
        return ret;
    }
    return true;
}

#define SYNC_MAT_PROP(s, member, dirty_flag)                                             \
    {                                                                                    \
        _NEW_STATIC_SYMBOL(s)                                                            \
        if (sym == _s) {                                                                 \
            Symbol action(#s "_edit_action");                                            \
            if (!(_op & (kPropSize | kPropGet)) && !IsEditable(action)) {                \
                return true;                                                             \
            }                                                                            \
            if (!PropSync(member, _val, _prop, _i + 1, _op))                             \
                return false;                                                            \
            if (!(_op & (kPropSize | kPropGet)))                                         \
                mDirty |= dirty_flag;                                                    \
            return true;                                                                 \
        }                                                                                \
    }

BEGIN_PROPSYNCS(RndMat)
    // clang-format off
    SYNC_PROP_SET(metamaterial, mMetaMaterial.Ptr(),
        mMetaMaterial = _val.Obj<MetaMaterial>();
        UpdatePropertiesFromMetaMat();
        unk225 = false;
    )
    // clang-format on
    SYNC_MAT_PROP(intensify, mIntensify, 2)
    SYNC_MAT_PROP(blend, (int &)mBlend, 2)
    SYNC_MAT_PROP(color, mColor, 1)
    SYNC_MAT_PROP(alpha, mColor.alpha, 1)
    SYNC_MAT_PROP(use_environ, mUseEnviron, 2)
    SYNC_MAT_PROP(z_mode, (int &)mZMode, 2)
    SYNC_MAT_PROP(stencil_mode, (int &)mStencilMode, 2)
    SYNC_MAT_PROP(tex_gen, (int &)mTexGen, 2)
    SYNC_MAT_PROP(tex_wrap, (int &)mTexWrap, 2)
    SYNC_MAT_PROP(tex_xfm, mTexXfm, 2)
    SYNC_MAT_PROP(diffuse_tex, mDiffuseTex, 2)
    SYNC_MAT_PROP(diffuse_tex2, mDiffuseTex2, 2)
    SYNC_MAT_PROP(prelit, mPrelit, 2)
    SYNC_MAT_PROP(alpha_cut, mAlphaCut, 2)
    SYNC_PROP(alpha_threshold, mAlphaThreshold)
    SYNC_MAT_PROP(alpha_write, mAlphaWrite, 2)
    SYNC_PROP(force_alpha_write, mAlphaWrite)
    SYNC_MAT_PROP(next_pass, mNextPass, 2)
    SYNC_MAT_PROP(cull, (int &)mCull, 2)
    SYNC_MAT_PROP(per_pixel_lit, mPerPixelLit, 2)
    SYNC_MAT_PROP(emissive_multiplier, mEmissiveMultiplier, 2)
    SYNC_MAT_PROP(specular_rgb, mSpecularRGB, 2)
    SYNC_MAT_PROP(specular_power, mSpecularRGB.alpha, 2)
    SYNC_MAT_PROP(specular2_rgb, mSpecular2RGB, 2)
    SYNC_MAT_PROP(specular2_power, mSpecular2RGB.alpha, 2)
    SYNC_MAT_PROP(normal_map, mNormalMap, 2)
    SYNC_MAT_PROP(emissive_map, mEmissiveMap, 2)
    SYNC_PROP_SET(
        specular_map,
        mSpecularMap.Ptr(),
        if (IsEditable("specular_map_edit_action")) SetSpecularMap(_val.Obj<RndTex>())
    )
    SYNC_MAT_PROP(environ_map, mEnvironMap, 2)
    SYNC_MAT_PROP(environ_map_falloff, mEnvironMapFalloff, 2)
    SYNC_MAT_PROP(environ_map_specmask, mEnvironMapSpecMask, 2)
    SYNC_MAT_PROP(de_normal, mDeNormal, 2)
    SYNC_MAT_PROP(anisotropy, mAnisotropy, 2)
    SYNC_MAT_PROP(norm_detail_tiling, mNormDetailTiling, 2)
    SYNC_MAT_PROP(norm_detail_strength, mNormDetailStrength, 2)
    SYNC_MAT_PROP(norm_detail_map, mNormDetailMap, 2)
    SYNC_MAT_PROP(rim_rgb, mRimRGB, 2)
    SYNC_MAT_PROP(rim_power, mRimRGB.alpha, 2)
    SYNC_MAT_PROP(rim_map, mRimMap, 2)
    SYNC_MAT_PROP(rim_light_under, mRimLightUnder, 2)
    SYNC_MAT_PROP(refract_enabled, mRefractEnabled, 2)
    SYNC_MAT_PROP(refract_strength, mRefractStrength, 2)
    SYNC_MAT_PROP(refract_normal_map, mRefractNormalMap, 2)
    SYNC_MAT_PROP(screen_aligned, mScreenAligned, 2)
    SYNC_MAT_PROP(shader_variation, (int &)mShaderVariation, 2)
    SYNC_MAT_PROP(point_lights, mPointLights, 2)
    SYNC_MAT_PROP(fog, mFog, 2)
    SYNC_MAT_PROP(fade_out, mFadeout, 2)
    SYNC_MAT_PROP(color_adjust, mColorAdjust, 2)
    SYNC_MAT_PROP(fur, mFur, 2)
    // more...
END_PROPSYNCS
