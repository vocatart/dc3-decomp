#pragma once
#include "obj/Data.h"
#include "rndobj/Dir.h"
#include "rndobj/Mat.h"
#include "rndobj/Mesh.h"
#include "ui/UIColor.h"
#include "ui/UILabel.h"
#include "ui/UIListWidget.h"
#include "utl/Symbol.h"

class UIList;
class UIListLabel;
class UIListMesh;
class UIListSubList;

class UIListProvider {
public:
    UIListProvider() {}
    virtual DataNode Handle(DataArray *, bool);
    virtual ~UIListProvider() {}
    virtual void Text(int, int, UIListLabel *, UILabel *) const;
    virtual RndMat *Mat(int, int, UIListMesh *) const;
    virtual UIListProvider *Provider(int, int, UIListSubList *) const { return nullptr; }
    virtual void Custom(int, int, class UIListCustom *, Hmx::Object *) const {}
    virtual void UpdateExtendedText(int, int, UILabel *) const;
    virtual void UpdateExtendedMesh(int, int, RndMesh *) const;
    virtual void UpdateExtendedCustom(int, int, Hmx::Object *) const;
    virtual Symbol DataSymbol(int) const { return gNullStr; }
    virtual bool CanSelect(int) const { return true; }
    virtual int DataIndex(Symbol s) const {
        int numdata = NumData();
        for (int i = 0; i < numdata; i++) {
            if (DataSymbol(i) == s)
                return i;
        }
        return -1;
    }
    virtual int NumData() const = 0;
    virtual bool IsActive(int) const { return true; }
    virtual bool IsHidden(int) const { return false; }
    virtual void InitData(RndDir *) {}
    virtual float GapSize(int, int, int, int) const { return 0; }
    virtual UIComponent::State
    ComponentStateOverride(int, int, UIComponent::State s) const {
        return s;
    }
    virtual UIListWidgetState ElementStateOverride(int, int, UIListWidgetState s) const {
        return s;
    }
    virtual UIListWidgetState
    SlotElementStateOverride(int, int, class UIListWidget *, UIListWidgetState s) const {
        return s;
    }
    virtual UIColor *SlotColorOverride(int, int, class UIListWidget *, UIColor *c) const {
        return c;
    }
    virtual void PreDraw(int, int, UIListSlot *) const {}
    virtual int SnappableAtOrBeforeData(int) const { return -1; }
    virtual bool IsSnappableAtData(int) const { return false; }
    virtual bool IsHeader(int) { return false; }
    virtual void UnHighlightCurrent() {}
    virtual void ClearIconLabels() {}
};
