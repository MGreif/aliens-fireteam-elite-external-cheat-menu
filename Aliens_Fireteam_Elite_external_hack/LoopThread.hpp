#pragma once
#ifndef AFE_LoopThread
#define AFE_LoopThread

#include "Memory.h"

typedef BOOL(*MemoryPatchCallback)(HANDLE hProcess, LPVOID baseAddress);
void startMemoryPatchLoop(LPVOID lpParam);

struct MemoryPatchLoopUnitOfWork {
    MemoryPatchCallback callback;
    BOOL active;
};

struct MemoryPatchLoopParam {
    HANDLE hProcess;
    LPVOID baseAddress;
    MemoryPatchLoopUnitOfWork** unitOfWorks; // Array of pointers
    UINT8 unitOfWorksSize;
};


#endif // !AFE_LoopThread
