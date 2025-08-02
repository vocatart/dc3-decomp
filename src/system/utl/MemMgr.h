#pragma once

void PhysDelta(const char *);
bool MemUseLowestMip();
int _GetFreePhysicalMemory();
int _GetFreeSystemMemory();
const char *MemHeapName(int);
int MemFindAddrHeap(void *);

void MemPushTemp();
void MemPopTemp();

void *MemAlloc(int size, const char *file, int line, const char *name, int align);
void MemFree(void *mem, const char *file, int line, const char *name);
void *MemOrPoolAlloc(int size, const char *file, int line, const char *name);
void *MemOrPoolAllocSTL(int size, const char *file, int line, const char *name);
void MemOrPoolFree(
    int,
    void *mem,
    const char *file = "unknown",
    int line = 0,
    const char *name = "unknown"
);
void MemOrPoolFreeSTL(int, void *mem, const char *file, int line, const char *name);

void *operator new(unsigned int);
void *operator new[](unsigned int);

void operator delete(void *v) { MemFree(v, "unknown", 0, "unknown"); }

#define NEW_OVERLOAD(class_name, line_num)                                               \
    void *operator new(unsigned int s) {                                                 \
        return MemAlloc(s, __FILE__, line_num, class_name, 0);                           \
    }                                                                                    \
    void *operator new(unsigned int s, void *place) { return place; }

// #define NEW_ARRAY_OVERLOAD                                                               \
//     void *operator new[](size_t t) { return _MemAlloc(t, 0); }                           \
//     void *operator new[](size_t, void *place) { return place; }

#define DELETE_OVERLOAD(class_name, line_num)                                            \
    void operator delete(void *v) { MemFree(v, __FILE__, line_num, class_name); }

// #define DELETE_ARRAY_OVERLOAD                                                            \
//     void operator delete[](void *v) { _MemFree(v); }
