#pragma once
#include "rndobj/Poll.h"

/** "A simple object with an enter an exit script call" */
class RndEnterable : public RndPollable {
public:
    OBJ_CLASSNAME(Enterable);
    OBJ_SET_TYPE(Enterable);
    virtual DataNode Handle(DataArray *, bool);
};
