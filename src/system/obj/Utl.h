#pragma once
#include "obj/Dir.h"
#include "utl/Symbol.h"
#include "obj/Data.h"

typedef bool FileCallbackFunc(char *);

void InitObject(Hmx::Object *);
// const char* PathName(const Hmx::Object*); // declared in ObjMacros.h instead, which you
// can just include obj/Object.h for anyway

/** Get this Object's name.
 * @param [in] obj The Object.
 * @returns The Object's name, or "NULL" if the object doesn't exist.
 */
const char *SafeName(Hmx::Object *obj);

/** Gets a list of every Object inside ObjectDir dir that is a subclass of class with name
 * parentSym. The list is in DataArray form, with each node containing the qualifying
 * Object's names.
 * @param [in] dir The ObjectDir to search for Objects in.
 * @param [in] parentSym The parent class in Symbol form to search for subclasses of.
 * @param [in] b TODO: currently unknown
 * @returns A DataNode housing the aforementioned DataArray.
 */
DataNode ObjectList(class ObjectDir *dir, Symbol parentSym, bool b);

/** Determine whether or not child is a subclass of parent.
 * @param [in] child The child class's name, in Symbol form.
 * @param [in] parent The potential parent class's name, in Symbol form.
 * @returns True if child is a subclass of parent, false if not.
 */
bool IsASubclass(Symbol child, Symbol parent);

DataNode MakeFileList(const char *, bool, FileCallbackFunc *);
DataNode MakeFileListFullPath(const char *);

/** Reserve enough entries in dst's hash and string tables to fit src's entries, plus any
 * extras.
 * @param [in] src The ObjectDir to determine how much space to reserve with.
 * @param [in] dst The ObjectDir to reserve entries in.
 * @param [in] extraObjects The number of extra objects to allocate space for in dst, in
 * addition to src's entries.
 */
void ReserveToFit(class ObjectDir *src, class ObjectDir *dst, int extraObjects);

const char *NextName(const char *, class ObjectDir *);

/** Replaces the Object from with to.
 * @param [in] from The Object to be replaced.
 * @param [in] to The Object serving as the replacement.
 * @param [in] copyDeep If true, the full Object will be copied over rather than just its
 * references.
 * @param [in] deleteFrom If true, delete from after the replacement has occurred.
 * @param [in] setProxyFile If true, set to's proxy file to from's - only used if copyDeep
 * is true!
 */
void ReplaceObject(
    Hmx::Object *from, Hmx::Object *to, bool copyDeep, bool deleteFrom, bool setProxyFile
);

const char *PrintPropertyPath(DataArray *);

// mergefilter classes go here
class MergeFilter {
public:
    enum Action {
        kMerge,
        kReplace,
        kKeep,
        kIgnore
    };
    enum Subdirs {
        kNoSubdirs,
        kAllSubdirs,
        kInlineSubdirs,
        kSubdir3,
        kSubdir4
    };

    MergeFilter() : mAction(kMerge), mSubdirs(kNoSubdirs) {}
    MergeFilter(Action a, Subdirs s) : mAction(a), mSubdirs(s) {}
    virtual ~MergeFilter() {}
    virtual Action Filter(Hmx::Object *, Hmx::Object *, class ObjectDir *) {
        return mAction;
    }
    virtual Action FilterSubdir(class ObjectDir *o1, class ObjectDir *) {
        return DefaultSubdirAction(o1, mSubdirs);
    }

    static Action DefaultSubdirAction(class ObjectDir *, Subdirs);

    Action mAction;
    Subdirs mSubdirs;
};

class DataMergeFilter : public MergeFilter {
public:
    DataMergeFilter(const DataNode &, Subdirs);
    virtual ~DataMergeFilter() {}
    virtual Action Filter(Hmx::Object *, Hmx::Object *, class ObjectDir *);

    DataType mType;
    DataFunc *mFunc;
    Hmx::Object *mObj;
    int mInt;
};

void MergeObject(Hmx::Object *, Hmx::Object *, ObjectDir *, MergeFilter::Action);
void MergeObject(Hmx::Object *, Hmx::Object *, ObjectDir *, MergeFilter &);
void MergeObjectsRecurse(ObjectDir *, ObjectDir *, MergeFilter &, bool);
void MergeDirs(ObjectDir *, ObjectDir *, MergeFilter &);
