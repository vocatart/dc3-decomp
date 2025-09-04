#include "gesture/ArcDetector.h"
#include "os/Debug.h"

float ArcDetector::_swipeRetentionFactor = 0.5;
float ArcDetector::_acceptablePathErrorRatio = 0.89999998;

ArcDetector::ArcDetector() : unk18(0, 0, 0) { Clear(); }
ArcDetector::~ArcDetector() {}

Vector3 ArcDetector::GetCurveStart() const {
    MILO_ASSERT(!mJointPath.empty(), 0xE9);
    return Vector3((mSide)*unk28, unk2c, 0);
}

void ArcDetector::Clear() {
    unk38 = 0;
    mJointPath.clear();
    unk2c = 0;
    unk28 = 0;
}

void ArcDetector::PrintJointPath() const {
    MILO_LOG("*** Hand path:\n");
    for (std::list<Vector3>::const_iterator it = mJointPath.begin();
         it != mJointPath.end();
         ++it) {
        MILO_LOG("%f, %f, %f,\n", it->x, it->y, it->z);
    }
    MILO_LOG("GetPathLength() %f\n", GetPathLength());
    MILO_LOG(
        "pow(GetPathLength(), _swipeRetentionFactor + 1) %f\n",
        pow(GetPathLength(), _swipeRetentionFactor + 1)
    );
    MILO_LOG("GetPathError() %f\n", GetPathError());
    MILO_LOG(
        "GetPathError() / _acceptablePathErrorRatio %f\n",
        GetPathError() / _acceptablePathErrorRatio
    );
    MILO_LOG("GetSwipeAmount() %f\n", GetSwipeAmount());
}

void ArcDetector::SwipeFailed(const Skeleton &skeleton) {
    if (unk38 > 0.5)
        unk35 = true;
    Vector3 vec = mJointPath.front();
    Clear();
    TryToStartSwipe(vec, skeleton);
}
