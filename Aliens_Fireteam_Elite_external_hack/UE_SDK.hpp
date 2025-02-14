#ifndef UE_SDK_MG
#define UE_SDK_MG
#pragma once

#include <stdio.h>
#include <Windows.h>
#include "memory.h"
#define NAME_LENGTH 256

class FNamePool
{
public:
    UINT64 pad;
    UINT32 wideCount;
    UINT32 ansiCount;
    char* chunks[1024]; // might be more
};

class FNameEntry {
public:
    UINT16 metadata; // 2 bytes containing length and some other meta
    char name[]; // Actual string

    const char* __thiscall getName() {
        return this->name;
    }
};

class FNamePoolIntern {
public:
    uintptr_t pFNamePool = NULL;
    FNamePoolIntern(uintptr_t _pFnamePool) {
        pFNamePool = _pFnamePool;
    }
    BOOL getName(HANDLE hProcess, UINT64 id, char name[NAME_LENGTH]);
};

class UObject {
public:
    uintptr_t vTable;
    UINT32 flags;
    UINT32 internalIndex;
    uintptr_t pClassPrivate;
    uintptr_t name;
    uintptr_t pClassOuter;
};


namespace UE_SDK {

    class Remote_SDK {
    public:
        uintptr_t pGObjectArray, pFNamePool, moduleBaseAddress = NULL;
        Mem* mem = nullptr;
        Remote_SDK(uintptr_t pGObjectArray, uintptr_t pFNamePool);
        void initMem(Mem* m);

        BOOL getFName(UINT64 id, char* out);
        UINT64 getFNameForUObject(uintptr_t uObject, char name[NAME_LENGTH]);
    };
    inline Remote_SDK* pSdk;

	void init();
    bool printAllFNames(HANDLE hProcess, LPVOID baseAddress);
    bool traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size);
    bool printFNameForUObjects(uintptr_t* uObjects, size_t size);
    bool printAllGObjects();
}



#endif