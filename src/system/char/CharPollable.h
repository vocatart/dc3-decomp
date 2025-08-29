#pragma once
#include "rndobj/Poll.h"
#include <list>

/** "Workhorse unit of the Character system, most Character things inherit from this." */
class CharPollable : public RndPollable {
public:
    CharPollable() {}
    virtual void PollDeps(std::list<Hmx::Object *> &, std::list<Hmx::Object *> &) = 0;
};
