#pragma once
#include "win_types.h"

struct __vector4 { /* Size=0x10 */
    union {
        /* 0x0000 */ float vector4_f32[4];
        /* 0x0000 */ DWORD vector4_u32[4];
        struct {
            /* 0x0000 */ float x;
            /* 0x0004 */ float y;
            /* 0x0008 */ float z;
            /* 0x000c */ float w;
        };
        /* 0x0000 */ float v[4];
        /* 0x0000 */ DWORD u[4];
    };
};
typedef __vector4 XMVECTOR;

struct XMVECTORF32 { /* Size=0x10 */
    union {
        /* 0x0000 */ float f[4];
        /* 0x0000 */ __vector4 v;
    };
};

struct XMVECTORI32 { /* Size=0x10 */
    union {
        /* 0x0000 */ int i[4];
        /* 0x0000 */ __vector4 v;
    };
};

struct XMVECTORU32 { /* Size=0x10 */
    union {
        /* 0x0000 */ unsigned int u[4];
        /* 0x0000 */ __vector4 v;
    };
};
