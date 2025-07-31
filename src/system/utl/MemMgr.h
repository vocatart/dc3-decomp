#pragma once

void PhysDelta(const char*);
bool MemUseLowestMip();
int _GetFreePhysicalMemory();
int _GetFreeSystemMemory();

void* operator new(unsigned int);
void* operator new[](unsigned int);

void* MemAlloc(int size, const char* file, int line, const char* name, int align);
void MemFree(void* mem, const char* file, int line, const char* name);
void* MemOrPoolAlloc(int size, const char* file, int line, const char* name);
void* MemOrPoolAllocSTL(int size, const char* file, int line, const char* name);
void MemOrPoolFree(int, void* mem, const char* file, int line, const char* name);
void MemOrPoolFreeSTL(int, void* mem, const char* file, int line, const char* name);

// #define NEW_OVERLOAD                                                                     \
//     void *operator new(size_t t) { return _MemAlloc(t, 0); }                             \
//     void *operator new(size_t, void *place) { return place; }

// #define NEW_ARRAY_OVERLOAD                                                               \
//     void *operator new[](size_t t) { return _MemAlloc(t, 0); }                           \
//     void *operator new[](size_t, void *place) { return place; }

// #define DELETE_OVERLOAD                                                                  \
//     void operator delete(void *v) { _MemFree(v); }

// #define DELETE_ARRAY_OVERLOAD                                                            \
//     void operator delete[](void *v) { _MemFree(v); }
