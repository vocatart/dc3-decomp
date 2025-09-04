#include "flow/FlowNode.h"
#include "flow/DrivenPropertyEntry.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/Debug.h"

float FlowNode::sIntensity = 1.0f;
bool FlowNode::sPushDrivenProperties = false;

FlowNode::FlowNode()
    : mVec1(this, (EraseMode)0, kObjListNoNull), mChildren(this), mParent(nullptr),
      unk48(this), unk58(0) {
    mDebugOutput = false;
}

FlowNode::~FlowNode() {
    if (!mChildren.empty()) {
        Deactivate(true);
    }
    for (ObjPtrVec<FlowNode>::iterator it = mVec1.begin(); it != mVec1.end(); ++it) {
        delete *it;
    }
}

BEGIN_HANDLERS(FlowNode)
    HANDLE_ACTION(activate, Activate());
    HANDLE_ACTION(deactivate, Deactivate(false));
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

BEGIN_PROPSYNCS(FlowNode)
    SYNC_PROP_SET(comment, Note(), SetNote(_val.Str()))
    SYNC_PROP(debug_output, mDebugOutput)
    SYNC_PROP(debug_comment, mDebugComment)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

void FlowNode::Save(BinStream &bs) {
    bs << 2;
    Hmx::Object::Save(bs);
}

void FlowNode::Load(BinStream &bs) {}

void FlowNode::MiloPreRun(void) {}

FlowNode *FlowNode::DuplicateChild(FlowNode *) { return nullptr; }

void FlowNode::Deactivate(bool) {
    if (mDebugOutput) {
        MILO_LOG("%s%s", ClassName(), MakeString("Deactivated\n"));
        if (!mDebugComment.empty()) {
            MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
        }
    }
}

void FlowNode::PushDrivenProperties(void) { sPushDrivenProperties = true; }

void FlowNode::MoveIntoDir(ObjectDir *, ObjectDir *) {}

void FlowNode::ActivateChild(FlowNode *child) {
    mChildren.push_back(child);
    if (!child->Activate()) {
        if (mDebugOutput) {
            MILO_LOG(
                "%s: %s",
                ClassName(),
                MakeString(
                    "Activated Child %s, which ran in full immediately.\n",
                    child->ClassName()
                )
            );
            if (!mDebugComment.empty()) {
                MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
            }
        }
        mChildren.remove(child);
    }
}

bool FlowNode::Activate() {
    if (mDebugOutput) {
        MILO_LOG("%s: %s", ClassName(), MakeString("Activating Children\n"));
        if (!mDebugComment.empty()) {
            MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
        }
    }
    return false;
}

void FlowNode::SetParent(class FlowNode *new_parent, bool b) {
    if (mParent != new_parent) {
        if (mParent != nullptr) {
            mParent->mVec1.remove(this);
        }
        mParent = new_parent;
        if (new_parent != nullptr && b) {
            new_parent->mVec1.push_back(this);
        }
    }
}

bool FlowNode::HasRunningNode(FlowNode *node) {
    return mChildren.find(node) != mChildren.end();
}

void FlowNode::UpdateIntensity() {
    for (ObjPtrList<FlowNode>::iterator it = mChildren.begin(); it != mChildren.end();
         ++it) {
        (*it)->UpdateIntensity();
    }
}

void FlowNode::RequestStop() {
    if (mDebugOutput) {
        MILO_LOG("%s: %s", ClassName(), MakeString("RequestStop\n"));
        if (!mDebugComment.empty()) {
            MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
        }
    }
    unk58 = true;
    for (ObjPtrList<FlowNode>::iterator it = mChildren.begin(); it != mChildren.end();
         ++it) {
        (*it)->RequestStop();
    }
}

void FlowNode::RequestStopCancel() {
    if (mDebugOutput) {
        MILO_LOG("%s: %s", ClassName(), MakeString("RequestStopCancel\n"));
        if (!mDebugComment.empty()) {
            MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
        }
    }
    unk58 = false;
    for (ObjPtrList<FlowNode>::iterator it = mChildren.begin(); it != mChildren.end();
         ++it) {
        (*it)->RequestStopCancel();
    }
}

DrivenPropertyEntry *FlowNode::GetDrivenEntry(Symbol s) {
    DataArrayPtr ptr(new DataArray(1));
    ptr->Node(0) = s;
    return GetDrivenEntry(ptr);
}

void FlowNode::ChildFinished(FlowNode *node) {
    if (mDebugOutput) {
        MILO_LOG(
            "%s: %s",
            ClassName(),
            MakeString("Child Finished of class:%s\n", node->ClassName())
        );
        if (!mDebugComment.empty()) {
            MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
        }
    }
    mChildren.remove(node);
    if (mChildren.empty()) {
        if (mDebugOutput) {
            MILO_LOG("%s: %s", ClassName(), MakeString("Releasing\n"));
            if (!mDebugComment.empty()) {
                MILO_LOG("Debug comment: %s\n", mDebugComment.c_str());
            }
        }
        if (mParent)
            mParent->ChildFinished(this);
    }
}
