#include "obj/Task.h"

TaskMgr TheTaskMgr;

void TaskMgr::Terminate() { SetName(nullptr, nullptr); }

TaskTimeline::TaskTimeline() : mTime(0), mLastTime(0), mPollingTask(nullptr) {}
TaskTimeline::~TaskTimeline() {}
