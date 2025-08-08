#pragma once
#include "obj/Object.h"
#include "utl/MemMgr.h"

enum TaskUnits {
    kTaskSeconds = 0,
    kTaskBeats = 1,
    kTaskUISeconds = 2,
    kTaskTutorialSeconds = 3,
    kTaskNumUnits = 4
};

/**
 * @brief Generic task type for simplistic multithreading.
 * Usually used for non-realtime tasks, such as scripting, message passing, etc
 */
class Task : public Hmx::Object {
public:
    Task() {}
    virtual ~Task() {}
    virtual void Poll(float) = 0;

    NEW_OVERLOAD("Task", 0x1A);
    DELETE_OVERLOAD("Task", 0x1A);
};

class TaskTimeline {
public:
    struct TaskInfo {};

    TaskTimeline();
    ~TaskTimeline();
    void Poll();
    void AddTask(Task *, float);
    void ClearTasks();
    void ResetTaskTime(float);
    void AddTask(const TaskInfo &);

private:
    std::list<TaskInfo> mTasks; // 0x0
    std::list<TaskInfo> mAddedTasks; // 0x8
    float mTime; // 0x10
    float mLastTime; // 0x14
    Task *mPollingTask; // 0x18
};

class TaskMgr : public Hmx::Object {
public:
    enum TimeReference {
        kDelayedTime = 0,
        kRealTime = 1
    };

    TaskMgr();
    virtual ~TaskMgr();
    virtual DataNode Handle(DataArray *, bool);

    float UISeconds() const;
    float DeltaSeconds() const;
    float DeltaBeat() const;
    float DeltaUISeconds() const;
    float Beat() const;
    float Seconds(TimeReference) const;
    float TutorialSeconds() const;
    float DeltaTime(TaskUnits) const;
    void Start(Task *, TaskUnits, float);
    float Time(TaskUnits) const;
    const char *GetMBT();
    void ClearTasks();
    void SetSeconds(float, bool);
    void SetDeltaTime(TaskUnits, float);
    void SetTimeAndDelta(TaskUnits, float, float);
    void Poll();
    void Init();
    void Terminate();
    void SetUISeconds(float, bool);
    void SetSecondsAndBeat(float, float, bool);
    void SetAVOffset(float);
    float DeltaTutorialSeconds() const;
    void ClearTimelineTasks(TaskUnits);

    DataNode OnTimeTilNext(DataArray *);
    // const SongPos &GetSongPos() const { return mSongPos; }

    // TaskTimeline *mTimelines; // 0x1c
    // SongPos mSongPos; // 0x20
    // bool mAutoSecondsBeats; // 0x34
    // Timer mTime; // 0x38
    // float mAVOffset; // 0x68
};

extern TaskMgr TheTaskMgr;
