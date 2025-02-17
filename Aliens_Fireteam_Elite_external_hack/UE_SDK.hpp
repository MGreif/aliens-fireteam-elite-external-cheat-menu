#ifndef UE_SDK_MG
#define UE_SDK_MG
#pragma once

#include <stdio.h>
#include <Windows.h>
#include "memory.h"
#define NAME_LENGTH 256
#define FULL_NAME_LENGTH 1024
#define NAME_EMPTY "_"
class FNamePool
{
public:
    UINT64 pad;
    UINT32 wideCount;
    UINT32 ansiCount;
    char* chunks[1024]; // might be more
};


namespace UE_SDK {

    enum EType
    {
        ClassInstance,
        StructProperty
    };

    class UObject {
    public:
        uintptr_t vTable;
        UINT32 flags;
        UINT32 internalIndex;
        uintptr_t pClassPrivate;
        UINT32 name;
        UINT32 pad1;
        uintptr_t pClassOuter;
        //Own methods
        bool static isUObject(uintptr_t pTarget);
        EType type;
        UObject static from(UObject* pTarget);
        bool isEmpty();
        bool findName();
        char asciiName[NAME_LENGTH] = NAME_EMPTY;
    };

    class StructProperty {
    public:
    };

    class UProperty {
    public:
        uintptr_t vTable;
        uintptr_t pad1;
        uintptr_t pOwner;
        uintptr_t pad2;
        UINT64 flags;
        UINT64 name;
        char pad3[0x1C];
        UINT32 Internal_offset;
        char pad4[0x8];
        UProperty* pNext;
        //Own methods
        UProperty(uintptr_t pTarget) {
            this->isUProperty(pTarget);
        }
        bool static isUProperty(uintptr_t pTarget);
        bool findName();
        char asciiName[NAME_LENGTH] = NAME_EMPTY;
    };


    class Remote_SDK {
    public:
        uintptr_t pGObjectArray, pFNamePool = NULL;
        Mem* mem = nullptr;
        UObject** pUObjects;
        size_t pUObjectsSize = 0;
        Remote_SDK(uintptr_t pGObjectArray, uintptr_t pFNamePool);
        void initMem(Mem* m);

        BOOL getFName(UINT32 id, char* out);
        BOOL getFullFName(UObject* pUObject, char* out);
        UINT64 getFNameForUObject(uintptr_t uObject, char name[NAME_LENGTH]);
        UObject** buildUObjectArray(size_t amount);
    };
    inline Remote_SDK* pSdk;

	void init();
    bool printAllFNames(HANDLE hProcess, LPVOID baseAddress);
    bool traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel);
    bool printFNameForUObjects(uintptr_t* uObjects, size_t size);
}



#endif