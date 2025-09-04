#include "gesture/JointUtl.h"
#include "gesture/BaseSkeleton.h"
#include "os/Debug.h"
#include "utl/Str.h"

bool IsSkeletonBone(const char *name) {
    static const char *sBoneNames[] = {
        "bone_spine1.mesh",     "bone_spine2.mesh",
        "bone_neck.mesh",       "bone_head.mesh",
        "bone_L-upperArm.mesh", "bone_L-foreArm.mesh",
        "bone_L-hand.mesh",     "bone_L-middlefinger03.mesh",
        "bone_R-upperArm.mesh", "bone_R-foreArm.mesh",
        "bone_R-hand.mesh",     "bone_R-middlefinger03.mesh",
        "bone_L-thigh.mesh",    "bone_L-knee.mesh",
        "bone_L-ankle.mesh",    "bone_R-thigh.mesh",
        "bone_R-knee.mesh",     "bone_R-ankle.mesh",
        "bone_L-toe.mesh",      "bone_R-toe.mesh"
    };
    for (const char **ptr = sBoneNames; *ptr != 0; ptr++) {
        if (streq(*ptr, name))
            return true;
    }
    return false;
}

const char *JointName(SkeletonJoint joint) {
    static const char *sJointNames[] = {
        "Hip Center",  "Spine",      "Shoulder Center", "Head",           "Left Shoulder",
        "Left Elbow",  "Left Wrist", "Left Hand",       "Right Shoulder", "Right Elbow",
        "Right Wrist", "Right Hand", "Left Hip",        "Left Knee",      "Left Ankle",
        "Right Hip",   "Right Knee", "Right Ankle",     "Left Foot",      "Right Foot"
    };
    MILO_ASSERT((0) <= (joint) && (joint) < (( sizeof(sJointNames)/sizeof(sJointNames[0])  )), 0x99);
    return sJointNames[joint];
}

const char *CharBoneName(SkeletonJoint joint) {
    static const char *sCharBoneNames[] = {
        "bone_spine1.mesh",     "bone_spine2.mesh",
        "bone_neck.mesh",       "bone_head.mesh",
        "bone_L-upperArm.mesh", "bone_L-foreArm.mesh",
        "bone_L-hand.mesh",     "bone_L-middlefinger03.mesh",
        "bone_R-upperArm.mesh", "bone_R-foreArm.mesh",
        "bone_R-hand.mesh",     "bone_R-middlefinger03.mesh",
        "bone_L-thigh.mesh",    "bone_L-knee.mesh",
        "bone_L-ankle.mesh",    "bone_R-thigh.mesh",
        "bone_R-knee.mesh",     "bone_R-ankle.mesh",
        "bone_L-toe.mesh",      "bone_R-toe.mesh"
    };
    MILO_ASSERT((0) <= (joint) && (joint) < (( sizeof(sCharBoneNames)/sizeof(sCharBoneNames[0]) )), 0xA0);
    return sCharBoneNames[joint];
}

const char *MirrorBoneName(SkeletonJoint joint) {
    static const char *sMirrorBoneNames[] = {
        "bone_spine1.mesh",     "bone_spine2.mesh",
        "bone_neck.mesh",       "bone_head.mesh",
        "bone_R-upperArm.mesh", "bone_R-foreArm.mesh",
        "bone_R-hand.mesh",     "bone_R-middlefinger03.mesh",
        "bone_L-upperArm.mesh", "bone_L-foreArm.mesh",
        "bone_L-hand.mesh",     "bone_L-middlefinger03.mesh",
        "bone_R-thigh.mesh",    "bone_R-knee.mesh",
        "bone_R-ankle.mesh",    "bone_L-thigh.mesh",
        "bone_L-knee.mesh",     "bone_L-ankle.mesh",
        "bone_R-toe.mesh",      "bone_L-toe.mesh"
    };
    // ghidra also has "camera", "left_arm", "right_arm",  "left_leg", "right_leg",
    // "pelvis"...but not sure why they're there or where they'd be used
    MILO_ASSERT((0) <= (joint) && (joint) < (( sizeof(sMirrorBoneNames)/sizeof(sMirrorBoneNames[0]) )), 0xA7);
    return sMirrorBoneNames[joint];
}

int JointParent(SkeletonJoint joint) {
    static int sJointParents[] = { -1, 0,   1, 2,   2,   4, 5,   6,    2,   8,
                                   9,  0xa, 0, 0xc, 0xd, 0, 0xf, 0x10, 0xe, 0x11 };
    MILO_ASSERT((0) <= (joint) && (joint) < (( sizeof(sJointParents)/sizeof(sJointParents[0]) )), 0xB5);
    return sJointParents[joint];
}
