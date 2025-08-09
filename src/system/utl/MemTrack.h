#pragma once

void MemTrackReportMemoryAlloc(const char *);
void MemTrackReportMemoryUsage(const char *);
void MemTrackReportClose(const char *);

void *DebugHeapAlloc(int);
void DebugHeapFree(void *);
