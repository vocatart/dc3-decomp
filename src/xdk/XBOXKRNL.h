#pragma once

// size 0x1C
struct CRITICAL_SECTION {
    void *DebugInfo; // was PRTL_CRITICAL_SECTION_DEBUG
    long LockCount; // signed 32-bit
    long RecursionCount; // signed 32-bit
    void *OwningThread; // thread handle or ID (pointer-sized)
    void *LockSemaphore; // handle to semaphore (pointer-sized)
    unsigned long SpinCount; // 32-bit on 32-bit builds
    int dummy; // to inflate the size
};

#ifdef __cplusplus
extern "C" {
#endif

void RtlInitializeCriticalSection(CRITICAL_SECTION *);
void RtlEnterCriticalSection(CRITICAL_SECTION *);
void RtlLeaveCriticalSection(CRITICAL_SECTION *);
int RtlTryEnterCriticalSection(CRITICAL_SECTION *);

#ifdef __cplusplus
}
#endif
