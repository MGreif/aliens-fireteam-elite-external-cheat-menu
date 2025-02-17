#pragma once
#ifndef UE_SDK_MG
#define UE_SDK_MG

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

/// <summary>
/// Small amateur algorithm to get the size of an array.
/// </summary>
/// <typeparam name="T">Type of the array items</typeparam>
/// <param name="arr">Array</param>
/// <returns>Size of the array</returns>
template <typename T>
UINT64 getSizeOfArray(T* arr) {
    UINT64 count = 0;
    T empty = { 0 };
    do {
        T* temp = arr + count;

        if (memcmp(temp, &empty, sizeof(T)) == 0) {
            return count;
        }
        count++;

    } while (true);
}


namespace UE_SDK {

    enum EType
    {
        ClassInstance,
        StructProperty
    };

    /// <summary>
    /// Base Class for UObjects
    /// </summary>
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
        char asciiName[NAME_LENGTH] = {0};
    };

    class StructProperty {
    public:
    };

    /// <summary>
    /// Base Class for UProperties. Also contains internal logic
    /// </summary>
    class UProperty {
    public:
        uintptr_t vTable;
        uintptr_t pad1;
        uintptr_t pOwner;
        uintptr_t pad2;
        UINT64 flags;
        UINT64 name;
        char pad3[0x1C] = {0};
        UINT32 Internal_offset;
        char pad4[0x8] = { 0 };
        UProperty* pNext;
        //Own methods
        UProperty(uintptr_t pTarget) {
            this->isUProperty(pTarget);
        }
        UProperty() {

        }
        bool isEmpty();
        bool static isUProperty(uintptr_t pTarget);
        bool findName();
        char asciiName[NAME_LENGTH] = { 0 };
    };

    /// <summary>
    /// Base class for SDK usage. Takes care of providing the correct memory adapter and storing global values like GObjectArray and FNamePool pointers
    /// </summary>
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
    bool traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel, bool onlyProperties);
    bool printFNameForUObjects(uintptr_t* uObjects, size_t size);
    bool getAllPropertiesForUObject(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel, char* prefix);
    UProperty* getPropertiesForUObject(uintptr_t _pUObject, size_t size);
}



#endif