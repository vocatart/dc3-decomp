#include "rndobj/Dir.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "rndobj/Draw.h"
#include "rndobj/Trans.h"
#include "utl/BinStream.h"

void RndDir::Save(BinStream &bs) {
    bs << 10;
    SAVE_SUPERCLASS(ObjectDir)
    SAVE_SUPERCLASS(RndAnimatable)
    SAVE_SUPERCLASS(RndDrawable)
    SAVE_SUPERCLASS(RndTransformable)
    bs << mEnv << mTestEvent;
}

BEGIN_COPYS(RndDir)
    COPY_SUPERCLASS(ObjectDir)
    COPY_SUPERCLASS(RndAnimatable)
    COPY_SUPERCLASS(RndDrawable)
    COPY_SUPERCLASS(RndTransformable)
    CREATE_COPY(RndDir)
    BEGIN_COPYING_MEMBERS
        if (ty != kCopyFromMax) {
            COPY_MEMBER(mEnv)
            COPY_MEMBER(mTestEvent)
        }
    END_COPYING_MEMBERS
END_COPYS

BEGIN_PROPSYNCS(RndDir)
    SYNC_PROP(environ, mEnv)
    SYNC_PROP(polls, mPolls)
    SYNC_PROP(enters, mEnters)
    SYNC_PROP(draws, mDraws)
    SYNC_PROP(test_event, mTestEvent)
    SYNC_SUPERCLASS(RndTransformable)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(RndAnimatable)
    SYNC_SUPERCLASS(ObjectDir)
END_PROPSYNCS

RndDir::RndDir() : mEnv(this) {}
