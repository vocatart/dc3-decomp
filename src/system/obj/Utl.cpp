#include "obj/Utl.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/File.h"
#include "os/System.h"
#include <cstdio>

std::list<String> sFilePaths;
std::list<Symbol> sFiles;

const char *PathName(const Hmx::Object *o) {
    return !o ? "NULL Object" : ((Hmx::Object *)o)->FindPathName();
}

MergeFilter::SubdirAction
MergeFilter::DefaultSubdirAction(ObjectDir *dir, Subdirs subdirs) {
    switch (subdirs) {
    case kNoSubdirs:
        return kKeep;
    case kAllSubdirs:
        return kMerge;
    case kSubdir3:
        return kReplace;
    case kInlineSubdirs:
        if (dir->InlineSubDirType() == kInlineNever
            || dir->InlineSubDirType() == kInlineCachedShared)
            return kKeep;
    case kSubdir4:
        if (dir->InlineSubDirType() == kInlineNever
            || dir->InlineSubDirType() == kInlineCachedShared)
            return kReplace;
    default:
        break;
    }
    return kMerge;
}

bool RecurseSuperClassesSearch(Symbol classSym, Symbol searchClass) {
    DataArray *classCfg = SystemConfig("objects")->FindArray(classSym, false);
    if (!classCfg)
        return false;
    DataArray *parents = classCfg->FindArray("superclasses", false);
    if (parents) {
        for (int i = 1; i < parents->Size(); i++) {
            Symbol curParent = parents->Sym(i);
            if (searchClass == curParent)
                return true;
            if (RecurseSuperClassesSearch(curParent, searchClass))
                return true;
        }
    }
    return false;
}

bool IsASubclass(Symbol child, Symbol parent) {
    if ((parent == "Object") || (child == parent))
        return true;
    else
        return RecurseSuperClassesSearch(child, parent);
}

const char *ClassExt(Symbol s) {
    static Symbol objects("objects");
    DataArray *cfg = SystemConfig(objects, s);
    DataArray *arr = cfg->FindArray("ext", false);
    return arr ? arr->Str(1) : nullptr;
}

void AddClassExt(char *file, Symbol s) {
    const char *ext = FileGetExt(file);
    if (*ext == '\0') {
        ext = ClassExt(s);
        if (ext) {
            strcat(file, ".");
            strcat(file, ext);
        }
    }
}

void MergeObject(
    Hmx::Object *o1, Hmx::Object *o2, ObjectDir *dir, MergeFilter::Action act
) {
    if (o1 == o2 || act == 3)
        return;
    if (o2) {
        o1->ReplaceRefs(o2);
        if (act == 0)
            o2->Copy(o1, Hmx::Object::kCopyFromMax);
        else if (act == 1)
            o2->Copy(o1, Hmx::Object::kCopyDeep);
    } else if (act != 2) {
        o1->SetName(o1->Name(), dir);
    }
}

int SubDirStringUsed(ObjectDir *dir) {
    if (!dir)
        return 0;
    else {
        int size = dir->StrTableUsedSize();
        const std::vector<ObjDirPtr<ObjectDir> > &subdirs = dir->SubDirs();
        for (std::vector<ObjDirPtr<ObjectDir> >::const_iterator it = subdirs.begin();
             it != subdirs.end();
             ++it) {
            size += SubDirStringUsed(*it);
        }
        return size;
    }
}

const char *NextName(const char *old_name, ObjectDir *dir) {
    if (!dir->FindObject(old_name, false, true))
        return old_name;
    const char *base = FileGetBase(old_name);
    const char *ext = FileGetExt(old_name);
    char *ptr;
    for (ptr = (char *)base + strlen(base); (base < ptr && (ptr[-1] - 0x30U <= 9)); ptr--)
        ;
    int atoied = 0;
    if (*ptr != '\0')
        atoied = atoi(ptr);
    char buf[128];
    do {
        atoied++;
        sprintf(ptr, "%02d", atoied);
        if (*ext != '\0') {
            sprintf(buf, "%s.%s", ptr, ext);
        } else {
            strcpy(buf, ptr);
        }
    } while (dir->FindObject(buf, false, true));

    return MakeString(buf);
}
