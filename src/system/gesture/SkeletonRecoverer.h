#pragma once
#include <list>

class SkeletonRecoverer {
public:
    struct TrackingIDHistory {
        int unk0;
    };
    SkeletonRecoverer();
    virtual ~SkeletonRecoverer();

protected:
    std::list<TrackingIDHistory> mIDHistory; // 0x4
};
