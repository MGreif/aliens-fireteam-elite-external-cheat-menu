#pragma once
#ifndef AFE_HackLogic
#define AFE_HackLogic

#include "..\Memory.h"

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

#endif // !AFE_HackLogic