#include "os/JoypadMsgs.h"
#include "os/Joypad.h"

ButtonDownMsg::ButtonDownMsg(LocalUser *user, JoypadButton butt, JoypadAction act, int i)
    : Message(Type(), user, butt, act, i) {}

ButtonUpMsg::ButtonUpMsg(LocalUser *user, JoypadButton butt, JoypadAction act, int i)
    : Message(Type(), user, butt, act, i) {}

JoypadConnectionMsg::JoypadConnectionMsg(LocalUser *user, bool b, int i, int j)
    : Message(Type(), user, b, i, j) {}

JoypadBreedDataReadMsg::JoypadBreedDataReadMsg(LocalUser *user, JoypadBreedDataStatus s)
    : Message(Type(), user, s) {}

JoypadBreedDataWriteMsg::JoypadBreedDataWriteMsg(LocalUser *user, JoypadBreedDataStatus s)
    : Message(Type(), user, s) {}
