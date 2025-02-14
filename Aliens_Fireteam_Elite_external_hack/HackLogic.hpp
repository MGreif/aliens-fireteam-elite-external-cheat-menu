#pragma once
#ifndef AFE_HackLogic
#define AFE_HackLogic

#include "Memory.h"

// Ammunition
BOOL setPrimaryMagazine(HANDLE hProcess, LPVOID baseAddress);
BOOL setPrimaryTotal(HANDLE hProcess, LPVOID baseAddress);
BOOL setSecondaryMagazine(HANDLE hProcess, LPVOID baseAddress);
BOOL setSecondaryTotal(HANDLE hProcess, LPVOID baseAddress);

// Recoil
BOOL setPrimaryNoRecoil(HANDLE hProcess, LPVOID baseAddress);
BOOL setSecondaryNoRecoil(HANDLE hProcess, LPVOID baseAddress);

// Spray
BOOL removeWeaponSpray(HANDLE hProcess, LPVOID baseAddress);

// Player speed
BOOL setWalkSpeedTo5000(HANDLE hProcess, LPVOID baseAddress);
BOOL setSprintSpeedTo5000(HANDLE hProcess, LPVOID baseAddress);
BOOL setFullStamina(HANDLE hProcess, LPVOID baseAddress);

// Player health

BOOL setHealthToMax(HANDLE hProcess, LPVOID baseAddress);

// Consumables
BOOL setIngameTopAndRightConsumableTo99(HANDLE hProcess, LPVOID baseAddress);
BOOL setIngameConsumablesTo999(HANDLE hProcess, LPVOID baseAddress);
BOOL setConsumablesTo999(HANDLE hProcess, LPVOID baseAddress); // only in hub world


// Entities
BOOL printEntitiesHealth(HANDLE hProcess, LPVOID baseAddress);

// Unreal

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
#define NAME_LENGTH 256
    BOOL getName(HANDLE hProcess, UINT64 id, char name[NAME_LENGTH]);
};

bool printAllFNames(HANDLE hProcess, LPVOID baseAddress);
bool printAllGObjects(HANDLE hProcess, LPVOID baseAddress);

BOOL printEntitiesNames(HANDLE hProcess, LPVOID baseAddress);
UINT64 getFNameForUObject(HANDLE hProcess, LPVOID baseAddress, uintptr_t uObject, char name[NAME_LENGTH]);
bool printFNameForUObjects(HANDLE hProcess, LPVOID baseAddress, uintptr_t* uObjects, size_t size);
bool traverseUObjectForMembersEtc(HANDLE hProcess, LPVOID baseAddress, uintptr_t _pUObject, size_t size);

#endif // !AFE_HackLogic