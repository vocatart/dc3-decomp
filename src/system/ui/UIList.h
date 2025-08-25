#pragma once
#include "ui/UIComponent.h"
#include "ui/UIListProvider.h"
#include "ui/ScrollSelect.h"
#include "ui/UIListState.h"
#include "ui/UITransitionHandler.h"

/**
 * @brief A UI Object representing a list element.
 * Original _objects description:
 * "Component for displaying 1- or 2-dimensional lists of data.
 * Can be oriented horizontally or vertically, can scroll normally or
 * circularly, and can have any number of visible elements (even just
 * one, a.k.a. a spin button)."
 */
class UIList : public UIComponent,
               public UIListProvider,
               public ScrollSelect,
               public UIListStateCallback,
               public UITransitionHandler {
public:
    // Hmx::Object
    virtual ~UIList() {}
    OBJ_CLASSNAME(UIList)
    OBJ_SET_TYPE(UIList)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndDrawable
    virtual float GetDistanceToPlane(const Plane &, Vector3 &);
    virtual void DrawShowing();
    virtual RndDrawable *CollideShowing(const Segment &, float &, Plane &);
    virtual int CollidePlane(const Plane &);
    // ScrollSelect
    virtual int SelectedAux() const;
    virtual void SetSelectedAux(int);
    // UIListStateCallback
    virtual void StartScroll(const UIListState &, int, bool);
    virtual void CompleteScroll(const UIListState &);
    // UITransitionHandler
    virtual void FinishValueChange();
    virtual bool IsEmptyValue() const;

protected:
    UIList();
};
