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
        if (unk224)
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
    : mMetaMaterial(this), unk20c(0), unk224(0), unk225(0), unk226(0), mDirty(3) {
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
