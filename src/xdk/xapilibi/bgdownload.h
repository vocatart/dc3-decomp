#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum _XBACKGROUND_DOWNLOAD_MODE {
    XBACKGROUND_DOWNLOAD_MODE_ALWAYS_ALLOW = 0x0001,
    XBACKGROUND_DOWNLOAD_MODE_AUTO = 0x0002,
};
typedef _XBACKGROUND_DOWNLOAD_MODE XBACKGROUND_DOWNLOAD_MODE;

DWORD XBackgroundDownloadSetMode(XBACKGROUND_DOWNLOAD_MODE);

#ifdef __cplusplus
}
#endif
