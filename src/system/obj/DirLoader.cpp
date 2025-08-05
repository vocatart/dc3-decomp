#include "obj/DirLoader.h"
#include "ObjRef.h"
#include "obj/Data.h"
#include "obj/ObjPtr_p.h"
#include "obj/Dir.h"
#include "os/Debug.h"
#include "os/File.h"
#include "os/System.h"
#include "utl/BinStream.h"
#include "utl/ChunkStream.h"

ObjectDir *DirLoader::GetDir() {
    MILO_ASSERT(IsLoaded(), 0x82);
    unk98 = true;
    return mDir;
}

void WriteDeadAndMark(BinStream &bs) {
    bs << (unsigned char)0xAD << (unsigned char)0xDE << (unsigned char)0xAD
       << (unsigned char)0xDE;
    MarkChunk(bs);
}

const char *DirLoader::CachedPath(const char *cc, bool b) {
    const char *ext = FileGetExt(cc);
    if ((sCacheMode || b) && ext) {
        if (streq(ext, "milo")) {
            return MakeString(
                "%s/gen/%s.milo_%s",
                FileGetPath(cc),
                FileGetBase(cc),
                PlatformSymbol(TheLoadMgr.GetPlatform())
            );
        }
    }
    return cc;
}

bool DirLoader::IsLoaded() const { return mState == &DirLoader::DoneLoading; }

bool DirLoader::ShouldBlockSubdirLoad(const FilePath &fp) {
    return fp.c_str() && sPathEval ? sPathEval(fp.c_str()) : false;
}

Symbol DirLoader::FixClassName(Symbol orig) {
    if (mRev < 0x1C) {
        static Symbol CharClip("CharClip");
        static Symbol CharClipSamples("CharClipSamples");
        if (orig == CharClipSamples) {
            orig = CharClip;
        }
    }
    return orig;
}

void ReadDead(BinStream &bs) {
    unsigned char val;
    bs >> val;
    while (true) {
        if (val == 0xAD) {
            bs >> val;
            if (val == 0xDE) {
                bs >> val;
                if (val == 0xAD) {
                    bs >> val;
                    if (val == 0xDE) {
                        return;
                    }
                }
            }
        } else {
            bs >> val;
        }
    }
}

void ReadEditorDirDead(BinStream &bs) {
    unsigned char buf;
    for (int i = 0; i < 20; i++) {
        while (true) {
            EofType t;
            while ((t = bs.Eof()) != NotEof) {
                MILO_ASSERT(t == TempEof, 0x470);
            }
            bs >> buf;
            if ("%#@EndOfEditorDir@#%"[i] == buf)
                break;
        }
    }
}

bool DirLoader::Replace(ObjRef *ref, Hmx::Object *obj) {
    //   if (param_1 == this + 0x84) {
    //     if (*(this + 0x90) != 0) {
    //       *(*(this + 0x8c) + 4) = *(this + 0x88);
    //       *(*(this + 0x88) + 8) = *(this + 0x8c);
    //     }
    //     pDVar2 = this + -0x1c;
    //     *(this + 0x90) = 0;
    //     *(this + 0x44) = 0;
    //     if (pDVar2 != 0x0) {
    //       (***pDVar2)(pDVar2,1,param_2);
    //     }
    //     bVar1 = true;
    //   }
    //   else {
    //     bVar1 = false;
    //   }
    //   return bVar1;
    return false;
}

void DirLoader::Cleanup(const char *str) {
    if (str) {
        MILO_WARN(str);
    }
    mObjects.clear();
    if (mOwnStream)
        RELEASE(mStream);
    if (mDir) {
        if (!IsLoaded()) {
            mDir->SetLoader(nullptr);
            if (!mProxyName) {
                RELEASE(mDir);
            }
        }
        if (mProxyName) {
            if (mDir->Dir() == mDir) {
                mDir->SetName(mProxyName, mProxyDir);
            }
        }
        if (IsLoaded() && mDir) {
            //   AutoGlitchReport::AutoGlitchReport(aAStack_1090,50.0,p_Var6,SyncObjectsGlitchCB);
            mDir->SetSubDirFlag(unk9b);
            mDir->SyncObjects();
            mDir->SetSubDirFlag(false);
            //   AutoGlitchReport::~AutoGlitchReport(aAStack_1090);
        }
    }
    mState = &DirLoader::DoneLoading;
    mTimer.Stop();
    if (sPrintTimes) {
        MILO_LOG("%s: %f ms\n", mFile, mTimer.Ms());
    }
}

DirLoader::~DirLoader() {
    mDeleteSelf = 0;
    if (!IsLoaded()) {
        Cleanup(nullptr);
    } else if (mDir) {
        mDir->SetLoader(nullptr);
        if (!unk98 && !mProxyName) {
            RELEASE(mDir);
        }
    }
    // if (mProxyDir)
    //     mProxyDir->Release(this);
    // if (mCallback && unk99) {
    //     // mCallback->FailedLoading(this);
    //     mCallback = 0;
    // }
}

DirLoader::DirLoader(
    const FilePath &fp,
    LoaderPos pos,
    Loader::Callback *cb,
    BinStream *stream,
    class ObjectDir *dir,
    bool bbb,
    class ObjectDir *dir2
)
    : Loader(fp, pos), mOwnStream(false), mStream(stream), mRev(0), mCounter(0),
      mObjects(nullptr, kObjListAllowNull), mCallback(cb), mDir(dir), mPostLoad(false),
      mLoadDir(false), mDeleteSelf(false), mProxyName(nullptr), unk98(0), unk99(0),
      unk9a(0), unk9b(bbb), unk9c(dir2), mProxyDir(this, nullptr) {
    if (mDir) {
        mDeleteSelf = true;
        mProxyName = mDir->Name();
        mProxyDir.SetObjConcrete(mDir);
        mDir->SetLoader(this);
    }
    if (!stream && !dir && !bbb) {
        DataArray *arr = SystemConfig()->FindArray("force_milo_inline", false);
        if (arr) {
            for (int i = 1; i < arr->Size(); i++) {
                const char *str = arr->Str(i);
                if (FileMatch(fp.c_str(), str)) {
                    MILO_FAIL("Can't dynamically load milo files matching %s", str);
                }
            }
        }
    }
    mState = &DirLoader::OpenFile;
}
