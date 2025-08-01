#pragma once
#include "os/System.h" /* IWYU pragma: keep */
#include "obj/PropSync_p.h" /* IWYU pragma: keep */
#include "obj/MessageTimer.h" /* IWYU pragma: keep */

namespace Hmx {
    class Object;
}

/** Get this Object's path name.
 * @param [in] obj The Object.
 * @returns The Object's path name, or "NULL Object" if it doesn't exist.
 */
const char *PathName(const class Hmx::Object *obj);

// BEGIN CLASSNAME MACRO
// -------------------------------------------------------------------------------

#define OBJ_CLASSNAME(classname)                                                         \
    virtual Symbol ClassName() const { return StaticClassName(); }                       \
    static Symbol StaticClassName() {                                                    \
        static Symbol name(#classname);                                                  \
        return name;                                                                     \
    }

// END CLASSNAME MACRO
// ---------------------------------------------------------------------------------

// BEGIN SET TYPE MACRO
// --------------------------------------------------------------------------------
#define OBJ_SET_TYPE(classname)                                                           \
    virtual void SetType(Symbol classname) {                                              \
        static DataArray *types = SystemConfig("objects", StaticClassName(), "types");    \
        if (classname.Null())                                                             \
            SetTypeDef(0);                                                                \
        else {                                                                            \
            DataArray *found = types->FindArray(classname, false);                        \
            if (found != 0)                                                               \
                SetTypeDef(found);                                                        \
            else {                                                                        \
                MILO_WARN(                                                                \
                    "%s:%s couldn't find type %s", ClassName(), PathName(this), classname \
                );                                                                        \
                SetTypeDef(0);                                                            \
            }                                                                             \
        }                                                                                 \
    }

// END SET TYPE MACRO
// ----------------------------------------------------------------------------------

// BEGIN HANDLE MACROS
// ---------------------------------------------------------------------------------

#define BEGIN_HANDLERS(objType)                                                          \
    DataNode objType::Handle(DataArray *_msg, bool _warn) {                              \
        Symbol sym = _msg->Sym(1);                                                       \
        MessageTimer timer(                                                              \
            (MessageTimer::Active()) ? static_cast<Hmx::Object *>(this) : 0, sym         \
        );

#define _HANDLE_CHECKED(expr)                                                            \
    {                                                                                    \
        DataNode result = expr;                                                          \
        if (result.Type() != kDataUnhandled)                                             \
            return result;                                                               \
    }

#define END_HANDLERS                                                                     \
    return DataNode(kDataUnhandled, 0);                                                  \
    }
