#include "ui/ScrollSelect.h"

void ScrollSelect::Store() { mSelectedAux = SelectedAux(); }
void ScrollSelect::Reset() { mSelectedAux = -1; }
bool ScrollSelect::CanScroll() const { return !mSelectToScroll || mSelectedAux != -1; }

ScrollSelect::ScrollSelect() : mSelectToScroll(0) { Reset(); }

DataNode ScrollSelect::SendScrollSelected(UIComponent *comp, LocalUser *user) {
    static UIComponentScrollSelectMsg scroll_select_msg(0, 0, 0);
    scroll_select_msg[0] = comp;
    scroll_select_msg[1] = user;
    scroll_select_msg[2] = mSelectedAux != -1;
    // return TheUI.Handle(scroll_select_msg, false);
    return 0;
}
