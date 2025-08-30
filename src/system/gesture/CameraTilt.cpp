#include "gesture/CameraTilt.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "os/PlatformMgr.h"
#include "os/System.h"
#include "ui/UI.h"
#include "xdk/NUI.h"

CameraTilt *TheCameraTilt;

void CameraTilt::UpdateTiltingToInital() {
    if (mOverlapped.InternalLow != 0x3E5) {
        MILO_LOG("NuiCameraAdjustTilt - completed tilt to inital\n");
        unk70 = 8;
        unk60 = 0;
    }
}

void CameraTilt::UpdateGetInitialTiltData() {
    if (mOverlapped.InternalLow != 0x3E5) {
        MILO_LOG("NuiCameraAdjustTilt - got initial tilt data\n");
        unk70 = 4;
        unk60 = 0;
    }
}

CameraTilt::CameraTilt()
    : unk2c(0), unk60(0), unk68(0), unk6c(0), unk70(0), unk74(0), mDelayBetweenStates(0),
      mDelayBetweenRetry(0), mUpDownCyclesPerScan(1), mAngleWiggleRoom(3),
      mErrorRepeatedTimes(0), mCycleSafetyTimeout(4), mTiltObjects(), unk180(0) {
    DataArray *camArr = SystemConfig()->FindArray("camera_tilt", false);
    if (camArr) {
        mDelayBetweenStates =
            camArr->FindInt("delay_between_states", mDelayBetweenStates);
        mDelayBetweenRetry = camArr->FindInt("delay_between_retry", mDelayBetweenRetry);
        mUpDownCyclesPerScan =
            camArr->FindInt("up_down_cycles_per_scan", mUpDownCyclesPerScan);
        mAngleWiggleRoom = camArr->FindInt("angle_wiggle_room", mAngleWiggleRoom);
        mErrorRepeatedTimes =
            camArr->FindInt("error_repeated_times", mErrorRepeatedTimes);
        mCycleSafetyTimeout =
            camArr->FindFloat("cycle_safety_timeout", mCycleSafetyTimeout);
    }
}

void CameraTilt::Init() {
    MILO_ASSERT(!TheCameraTilt, 0x36);
    TheCameraTilt = new CameraTilt();
    TheCameraTilt->SetName("camera_tilt", ObjectDir::Main());
}

void CameraTilt::StartCameraScan() {
    if (unk70 != 0) {
        MILO_LOG(
            "StartCameraScan: ERROR : Scan is trying to be initiated while in a scan sequence. Ignoring Scan Request!!!"
        );
    } else {
        unk70 = 1;
        unk2c = true;
        unk68 = 0;
        unk60 = 0;
        mTimer.Start();
        ThePlatformMgr.AddSink(TheCameraTilt);
    }
}

void CameraTilt::StartGetInitialTiltData() {
    DWORD ret =
        NuiCameraAdjustTilt(0x20, 0, 2.1336f, 2.1336f, &mTiltObjects, &mOverlapped);
    unk60 = 0;
    if (ret == 0) {
        unk70 = 3;
        MILO_LOG(
            "NuiCameraAdjustTilt completed immediately - camera tilt already optimal?\n"
        );
    } else if (ret == 0x3E5) {
        unk70 = 3;
        unk6c = unk74 == 1 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("NuiCameraAdjustTilt - Camera is getting initial camera data\n");
        }
        unk74 = 1;
    } else if (ret == 0x4D5) {
        unk70 = 2;
        unk6c = unk74 == 2 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("NuiCameraAdjustTilt called too soon after previous call\n");
        }
        unk74 = 2;
    } else if (ret == 0xAA) {
        unk70 = 2;
        unk6c = unk74 == 3 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("NuiCameraAdjustTilt failed because camera was busy\n");
        }
        unk74 = 3;
    } else if (ret == 0x38) {
        unk70 = 2;
        unk6c = unk74 == 4 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("NuiCameraAdjustTilt failed to find player candidate; waiting\n");
        }
        unk74 = 4;
    } else {
        unk70 = 0;
        unk6c = unk74 == 5 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("Unexpected result from NuiCameraAdjustTilt - %x\n", ret);
        }
        unk74 = 5;
    }
}

void CameraTilt::StartCameraTiltingUp() {
    HRESULT res = NuiCameraElevationSetAngle(27);
    if (res == 0) {
        MILO_LOG("NuiCameraElevationSetAngle - Camera is tilting to Up\n");
        unk70 = 15;
        unk60 = 0;
    } else if (res == 0x80070057) {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 6 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the input angle is outside the accepted range\n"
            );
        }
        unk74 = 6;
    } else if (res == 0x8007048F) {
        unk70 = 0;
        unk60 = 0;
        unk6c = unk74 == 7 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the Kinect sensor array is not attached\n"
            );
        }
        unk74 = 7;
    } else if (res == 0x8301000B) {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 8 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the Xbox Guide UI is active so elevation will not be changed\n"
            );
        }
        unk74 = 8;
    } else {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 5 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("Unexpected result from NuiCameraElevationSetAngle - %x\n", res);
        }
        unk74 = 5;
    }
}

void CameraTilt::StartCameraTiltingDown() {
    HRESULT res = NuiCameraElevationSetAngle(-27);
    if (res == 0) {
        MILO_LOG("NuiCameraElevationSetAngle - Camera is tilting to Down\n");
        unk70 = 11;
        unk60 = 0;
    } else if (res == 0x80070057) {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 6 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the input angle is outside the accepted range\n"
            );
        }
        unk74 = 6;
    } else if (res == 0x8007048F) {
        unk70 = 0;
        unk60 = 0;
        unk6c = unk74 == 7 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the Kinect sensor array is not attached\n"
            );
        }
        unk74 = 7;
    } else if (res == 0x8301000B) {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 8 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG(
                "NuiCameraElevationSetAngle failed because the Xbox Guide UI is active so elevation will not be changed\n"
            );
        }
        unk74 = 8;
    } else {
        unk70 = 5;
        unk60 = 0;
        unk6c = unk74 == 5 ? unk6c + 1 : 0;
        if (unk6c <= mErrorRepeatedTimes) {
            MILO_LOG("Unexpected result from NuiCameraElevationSetAngle - %x\n", res);
        }
        unk74 = 5;
    }
}

DataNode CameraTilt::OnMsg(const UIChangedMsg &msg) {
    if (unk70 != 0) {
        if (!msg.Showing()) {
            unk2c = true;
            mTimer.Start();
        } else {
            unk2c = false;
            mTimer.Stop();
        }
    }
    return 0;
}

BEGIN_HANDLERS(CameraTilt)
    HANDLE_ACTION(camera_scan, StartCameraScan())
    HANDLE_MESSAGE(UIChangedMsg)
END_HANDLERS

BEGIN_PROPSYNCS(CameraTilt)
    SYNC_PROP(angle, mAngle)
END_PROPSYNCS
