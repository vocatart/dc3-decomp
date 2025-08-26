#pragma once
#include "obj/Msg.h"
#include "os/User.h"
#include "os/Joypad.h"

DECLARE_MESSAGE(ButtonDownMsg, "button_down")
ButtonDownMsg(LocalUser *, JoypadButton, JoypadAction, int);
LocalUser *GetUser() const;
JoypadButton GetButton() const { return (JoypadButton)mData->Int(3); }
JoypadAction GetAction() const { return (JoypadAction)mData->Int(4); }
int GetPadNum() const { return mData->Int(5); }
END_MESSAGE

DECLARE_MESSAGE(ButtonUpMsg, "button_up")
ButtonUpMsg(LocalUser *, JoypadButton, JoypadAction, int);
LocalUser *GetUser() const;
JoypadButton GetButton() const { return (JoypadButton)mData->Int(3); }
JoypadAction GetAction() const { return (JoypadAction)mData->Int(4); }
int GetPadNum() const { return mData->Int(5); }
END_MESSAGE

DECLARE_MESSAGE(JoypadConnectionMsg, "joypad_connect")
JoypadConnectionMsg(LocalUser *user, bool connected, int, int);
LocalUser *GetUser() const;
bool Connected() const { return mData->Int(3); }
END_MESSAGE

DECLARE_MESSAGE(JoypadBreedDataReadMsg, "joypad_breed_data_read")
JoypadBreedDataReadMsg(LocalUser *, JoypadBreedDataStatus);
END_MESSAGE

DECLARE_MESSAGE(JoypadBreedDataWriteMsg, "joypad_breed_data_write")
JoypadBreedDataWriteMsg(LocalUser *, JoypadBreedDataStatus);
END_MESSAGE
