#include "obj/Dir.h"
#include "Object.h"

void ObjectDir::SetCurViewport(ObjectDir::ViewportId id, Hmx::Object *o) {
    mCurViewportID = id;
    mCurCam = o;
}

ObjectDir::ObjectDir() : mHashTable(0, Entry(), Entry(), 0), mStringTable(0) {}
