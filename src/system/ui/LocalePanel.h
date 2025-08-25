#pragma once
#include "ui/UIListProvider.h"
#include "ui/UIPanel.h"

class LocalePanel : public UIPanel, public UIListProvider {
public:
    struct Entry {
        Entry();
        String mHeading; // 0x0
        String mLabel; // 0x8
        Symbol mToken; // 0x10
        String mString; // 0x14
    };
    LocalePanel();
    // Hmx::Object
    virtual ~LocalePanel();
    OBJ_CLASSNAME(LocalePanel)
    OBJ_SET_TYPE(LocalePanel)
    virtual DataNode Handle(DataArray *, bool);
    // UIPanel
    virtual void Enter();
    // UIListProvider
    virtual void Text(int, int, UIListLabel *, UILabel *) const;
    virtual int NumData() const;
    virtual bool IsActive(int) const;
    virtual float GapSize(int, int, int, int) const;

protected:
    std::vector<Entry> mEntries; // 0x3c
};
