#pragma once
#include <vector>

class UIListProvider;
class UIListStateCallback;

class UIListState {
public:
    UIListState(UIListProvider *, UIListStateCallback *);

    void SetNumDisplay(int, bool);
    void SetGridSpan(int, bool);
    void SetSelected(int, int, bool);

private:
    /** "Does the list scrolling wrap?" */
    bool mCircular; // 0x0
    int mNumDisplay; // 0x4
    int mGridSpan; // 0x8
    float mSpeed; // 0xc
    /** "How far from top of list to start scrolling". Range from 0 to 50 */
    int mMinDisplay; // 0x10
    bool mScrollPastMinDisplay; // 0x14
    /** "How far down can the highlight travel before scoll? Use -1 for no limit". Range
     * from -1 to 50 */
    int mMaxDisplay; // 0x18
    /** "Allow selected data to move beyond max highlight?" */
    bool mScrollPastMaxDisplay; // 0x1c
    UIListProvider *mProvider; // 0x20
    std::vector<int> mHiddenData; // 0x24
    int mFirstShowing; // 0x30
    int mTargetShowing; // 0x34
    int mSelectedDisplay; // 0x38
    float mStepPercent; // 0x3c
    float mStepTime; // 0x40
    UIListStateCallback *mCallback; // 0x44
};

class UIListStateCallback {
public:
    virtual ~UIListStateCallback() {}
    virtual void StartScroll(const UIListState &, int, bool) = 0;
    virtual void CompleteScroll(const UIListState &) = 0;
};
