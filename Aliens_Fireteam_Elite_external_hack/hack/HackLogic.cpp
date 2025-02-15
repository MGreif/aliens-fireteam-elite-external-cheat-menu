#include <iostream>
#include "..\Memory.h"
#include "HackLogic.hpp"
#include "game_pointers.hpp"


// Ammunition

BOOL setPrimaryMagazine(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting current magazine ammo to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 };  // decimal 9999 little-endian
    if (!writeMemory(hProcess, baseAddress, primaryMagazinePointerchain, primaryMagazinePointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    return true;
}

BOOL setPrimaryTotal(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting total ammo for current weapon to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 }; // decimal 9999 little-endian
    if (!writeMemory(hProcess, baseAddress, primaryTotalAmmoPointerchain, primaryTotalAmmoPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    return true;
}



BOOL setSecondaryMagazine(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting current magazine ammo to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 }; // decimal 9999 little-endian
    if (!writeMemory(hProcess, baseAddress, secondaryMagazinePointerchain, secondaryMagazinePointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    return true;
}

BOOL setSecondaryTotal(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting total ammo for current weapon to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 }; // decimal 9999 little-endian
    if (!writeMemory(hProcess, baseAddress, secondaryTotalAmmoPointerchain, secondaryTotalAmmoPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    return true;
}


// Recoil

BOOL setSecondaryNoRecoil(HANDLE hProcess, LPVOID baseAddress) {

    char buffer[] = { 0x0, 0x0, 0x0, 0x0 };

    if (!writeMemory(hProcess, baseAddress, secondaryADSRecoilPointerchain, secondaryADSRecoilPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    if (!writeMemory(hProcess, baseAddress, secondaryNoADSRecoilPointerchain, secondaryNoADSRecoilPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
}

BOOL setPrimaryNoRecoil(HANDLE hProcess, LPVOID baseAddress) {

    char buffer[] = { 0x0, 0x0, 0x0, 0x0 };

    if (!writeMemory(hProcess, baseAddress, primaryADSRecoilPointerchain, primaryADSRecoilPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    if (!writeMemory(hProcess, baseAddress, primaryNoADSRecoilPointerchain, primaryNoADSRecoilPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
}


// Spray

BOOL removeWeaponSpray(HANDLE hProcess, LPVOID baseAddress) {

    uintptr_t onAimAddress = (uintptr_t)baseAddress + weaponSpreadPatchOnAim.address;

    if (!patchMemory(hProcess, (LPVOID)onAimAddress, weaponSpreadPatchOnAim)) {
        std::cout << "Could not patch memory ..." << std::endl;
        return false;
    }
    else {
        std::cout << "Patched spread reset on aim" << std::endl;
    }

    uintptr_t onShootAddress = (uintptr_t)baseAddress + weaponSpreadPatchOnShoot.address;

    if (!patchMemory(hProcess, (LPVOID)onShootAddress, weaponSpreadPatchOnShoot)) {
        std::cout << "Could not patch memory ..." << std::endl;
        return false;
    }
    else {
        std::cout << "Patched spread reset on shoot" << std::endl;
    }


    char buffer[] = { 0x0, 0x0, 0x0, 0x0 }; // 4 Byte
    if (!writeMemory(hProcess, baseAddress, secondarySpreadPointerchain, secondarySpreadPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    else {
        std::cout << "Set spread to 0" << std::endl;
    }

    if (!writeMemory(hProcess, baseAddress, primarySpreadPointerchain, primarySpreadPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    else {
        std::cout << "Set spread to 0" << std::endl;
    }

    return true;
}


// Player speed


BOOL setWalkSpeedTo5000(HANDLE hProcess, LPVOID baseAddress) {

    char buffer[] = { 0x0, 0x40, 0x9c, 0x45 }; // decimal 5000 little-endian

    if (!writeMemory(hProcess, baseAddress, maxWalkSpeedPointerchain, maxWalkSpeedPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
}


BOOL setSprintSpeedTo5000(HANDLE hProcess, LPVOID baseAddress) {

    char buffer[] = { 0x0, 0x40, 0x9c, 0x45 }; // decimal 5000 little-endian

    if (!writeMemory(hProcess, baseAddress, sprintSpeedPointerchain, sprintSpeedPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
}

BOOL setFullStamina(HANDLE hProcess, LPVOID baseAddress) {
    char buffer[] = { 0x0, 0x0, 0xc8, 0x42 }; // float 100.0f little-endian
    if (!writeMemory(hProcess, baseAddress, staminaPointerchain, staminaPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }
    return true;
}

// Player Health


BOOL setHealthToMax(HANDLE hProcess, LPVOID baseAddress) {
    char buffer[] = { 0x0, 0x80, 0xbb, 0x44 };
    if (!writeMemory(hProcess, baseAddress, ingameHealthPointerchain, ingameHealthPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }

    if (!writeMemory(hProcess, baseAddress, ingameHealthPointerchain2, ingameHealthPointerchain2Size, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return false;
    }

    return true;
}


// Consumables

BOOL setIngameTopAndRightConsumableTo99(HANDLE hProcess, LPVOID baseAddress) {

    if (!setIngameConsumablesTo999(hProcess, baseAddress)) {
        return 1;
    }

    char buffer[] = { 0x63, 0x0, 0x0, 0x0 };

    if (!writeMemory(hProcess, baseAddress, ingameConsumableTopAmountPointerchain, ingameConsumableTopAmountPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    if (!writeMemory(hProcess, baseAddress, ingameConsumableRightAmountPointerchain, ingameConsumableRightAmountPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
}


BOOL setConsumablesTo999(HANDLE hProcess, LPVOID baseAddress) {
    LPVOID* consumablePointerMaps[] = {
        normalSentryConsumablePointerchain,
        electroshockSentryConsumablePointerchain,
        incendiarySentryConsumablePointerchain,
        staticGridConsumablePointerchain,
        cryoGridConsumablePointerchain,
        mineConsumablePointerchain,
        electroshockMineConsumablePointerchain,
        phosphorusMineConsumablePointerchain,
        incendiaryRoundsConsumablePointerchain,
        electroshockRoundsConsumablePointerchain,
        droneConsumablePointerchain
    };
    char buffer[] = { 0xe7, 0x03 }; // decimal 999 little-endian

    for (int i = 0; i < sizeof(consumablePointerMaps) / sizeof(LPVOID*) - 1; i++) {
        if (!writeMemory(hProcess, baseAddress, consumablePointerMaps[i], consumablePointerchainSize, buffer, sizeof(buffer))) {
            std::cout << "Could not patch memory ...";
            return false;
        }
    }

    return true;
}

BOOL setIngameConsumablesTo999(HANDLE hProcess, LPVOID baseAddress) {
    LPVOID* consumablePointerMaps[] = {
        ingameNormalSentryConsumablePointerchain,
        ingameElectroshockSentryConsumablePointerchain,
        ingameIncendiarySentryConsumablePointerchain,
        staticGridConsumablePointerchain,
        cryoGridConsumablePointerchain,
        ingameMineConsumablePointerchain,
        ingameElectroshockMineConsumablePointerchain,
        ingamePhosphorusMineConsumablePointerchain,
        ingameIncendiaryRoundsConsumablePointerchain,
        ingameElectroshockRoundsConsumablePointerchain,
        ingameDroneConsumablePointerchain
    };

    std::cout << "Setting all consumables to 99 (0x63)" << std::endl;


    for (int i = 0; i < sizeof(consumablePointerMaps) / sizeof(LPVOID*) - 1; i++) {
        char buffer[] = { 0x63, 0x00 }; // decimal 99 little-endian
        if (!writeMemory(hProcess, baseAddress, consumablePointerMaps[i], consumablePointerchainSize, buffer, sizeof(buffer))) {
            std::cout << "Could not patch memory ...";
            return false;
        }
    }

    return true;
}

// Entities

BOOL printEntitiesHealth(HANDLE hProcess, LPVOID baseAddress) {
    EntityWrapper** pEntityList = reinterpret_cast<EntityWrapper**>(Mem::getDynamicMemoryAddress(hProcess, baseAddress, ingameEntityListPointerchain, ingameEntityListPointerchainSize));

    UINT8 uAmountEntities = 100;
    int count = 0;
    system("cls");
    for (UINT8 i = 0; i < uAmountEntities; i++) {


        // This reads the value that should point to the entity list (Array of pointers)
        EntityWrapper** ppEntityWrapper = nullptr;
        ReadProcessMemory(hProcess, pEntityList, &ppEntityWrapper, sizeof(EntityWrapper**), NULL);

        // Iterate through the list
        LPVOID pCurrentEntity = ppEntityWrapper + i; // + i as ppEntityWrapper is an array of pointers. So its + i(e.g 1) * 8
        //    printf("EntityWrapper* at %p\n", pCurrentEntity);

            // Read the value at the index of the list (Array of pointers)
        EntityWrapper* pEntityWrapper = { 0 };
        ReadProcessMemory(hProcess, pCurrentEntity, &pEntityWrapper, sizeof(EntityWrapper*), NULL);

        // Grab the EntityWrapper
        EntityWrapper entityWrapper = { 0 };
        ReadProcessMemory(hProcess, pEntityWrapper, &entityWrapper, sizeof(EntityWrapper), NULL);
        //    printf("Entity at: %p\n", entityWrapper.pEntity);

            // Grab the entity
        Entity entity = Entity();
        ReadProcessMemory(hProcess, entityWrapper.pEntity, &entity, sizeof(Entity), NULL);


        if (entity.fHealth <= 1.f) {
            //     printf("Probably found the end of the entity list ...\n");
            continue;
        }
        printf("Entity health: %f\n", entity.fHealth);

        if (i < 3) {
            //       printf("Continuing as entity might be a player or bot\n");
            continue;
        }


        count++;

        float buff = 0.f;
        if (!WriteProcessMemory(hProcess, (char*)entityWrapper.pEntity + 0x3c, &buff, sizeof(float), NULL)) {
            printf("Could not set health to 0.f\n");
        }
        else {
            printf("Set health to 0.f\n");
        }
    }

    printf("Found %d entities with health\n", count);
    return true;
}



/*


BOOL FNamePoolIntern::getName(HANDLE hProcess, UINT64 id, char name[NAME_LENGTH]) {
    UINT16 chunkId = id >> 16;
 //   printf("[%x] Chunk: %u, pFNamePool: %p\n", id, chunkId, pFNamePool);

    void* pChunk = RemoteMem().read<void*>(hProcess, (void**)pFNamePool);

  //  printf("[%x] pChunk: %p result: %p\n", id, pChunk, (uintptr_t)pChunk + (uintptr_t)0x10 + (uintptr_t)chunkId * 8);


    void* chunk = RemoteMem().read<void*>(hProcess, (void**)((char*)pChunk + (char)0x10 + (char)chunkId * 8));
 //   printf("[%x] chunkId: %d; chunk: %p\n", id, chunkId, chunk);


    UINT64 pFNameEntry =  0x000000000001FFFF & (id * 2);
 //   printf("[%x] nameOffset: %u\n", id, pFNameEntry);

    char* pName = (char*)((uintptr_t)chunk + (uintptr_t)pFNameEntry);
  //  printf("[%x] chunk(%p) + pFNameEntry (%p) + 0x2 = %p\n", id, chunk, pFNameEntry, pName);
 //  printf("[%x] pName: %p Length: %u\n", id, pName);

    char tempName[NAME_LENGTH] = { 0 };

    if (!ReadProcessMemory(hProcess, pName, tempName, NAME_LENGTH, NULL)) {
        printf("[!] Could not read memory! Error: %u\n", GetLastError());
        return false;
    }

    // Name legally contains / | . _ -


    int skip = 0;

    if (tempName[2] <= 0x2D)
    {
        tempName[2] = '_';
        skip = 3;
    } else if (tempName[1] <= 0x2D) {
        tempName[1] = '_';
        skip = 2;
    } else if (tempName[0] <= 0x2D) {
        tempName[0] = '_';
        skip = 1;
    }


    if (strncmp(tempName,"y",1) == 0) {
        return true;
    }

    for (int i = 2; i < strlen(tempName); i++) {
        if (tempName[i] <= 0x2D) {
            tempName[i] = 0x00;
        }
    }

    if (strlen((char*)tempName + skip) == 1) {
        return true;
    }

    memcpy(name, (char*)tempName + skip, strlen(tempName));
    if (strlen(name) == 0) {
        printf("[!] Not found: id:%u chunkId:%u pChunk:%p pFname: %p\n", id, chunkId, chunk, pName);
    }
    // printf("Name: %s\n", name);
    return true;
}




bool printAllFNames(HANDLE hProcess, LPVOID baseAddress) {
    LPVOID addr = getDynamicMemoryAddress(hProcess, baseAddress, FNamePoolPointerchain, FNamePoolPointerchainSize);
    FNamePoolIntern pool = FNamePoolIntern((uintptr_t)baseAddress + (uintptr_t)addr);
    char name[NAME_LENGTH] = { 0 };

    if (!pool.getName(hProcess, (UINT64)0x00061299, name)) {
        printf("Could not get id: %x", 0x00061299);
    }



    return true;
}


UINT64 getFNameForUObject(HANDLE hProcess, LPVOID baseAddress, uintptr_t uObject, char name[NAME_LENGTH]) {
    LPVOID FNamePool = getDynamicMemoryAddress(hProcess, baseAddress, FNamePoolPointerchain, FNamePoolPointerchainSize);
    FNamePoolIntern pool = FNamePoolIntern((uintptr_t)baseAddress + (uintptr_t)FNamePool);

    auto current = uObject;
    if (current < 0xFFFFFF || current > 0x7FF000000000) return 0;
    // printf("Found: %p\n", current);
    UINT64 nameIndex = RemoteMem().read<UINT64>(hProcess, (UINT64*)((char*)current + 0x18));
    if (nameIndex == NULL) {
        //  printf("[!] Not able to get nameindex!\n");
        return 0;
    }

    if (nameIndex > 0xFFFFFFF) {
        //  printf("[!] NameIndex too big!\n");
        return 0;
    }

    //  printf("Found nameIndex: %p at: %p\n", nameIndex, (UINT64*)((char*)current + 0x18));

    pool.getName(hProcess, nameIndex, name);


    return nameIndex;
}

bool printFNameForUObjects(HANDLE hProcess, LPVOID baseAddress, uintptr_t* uObjects, size_t size) {
    LPVOID FNamePool = getDynamicMemoryAddress(hProcess, baseAddress, FNamePoolPointerchain, FNamePoolPointerchainSize);
    FNamePoolIntern pool = FNamePoolIntern((uintptr_t)baseAddress + (uintptr_t)FNamePool);

    printf("[index] [nameIndex] [pUObject] name\m");
    for (int i = 0; i < size; i++) {
        auto current = uObjects[i];

        // Get vtablePointer to validate if UObject is a class
        uintptr_t vtablePointer = RemoteMem().read<uintptr_t>(hProcess, (uintptr_t*)current);
        if (vtablePointer < 0x00007FF000000000) continue;
        char name[NAME_LENGTH] = { 0 };

        UINT64 nameIndex = getFNameForUObject(hProcess, baseAddress, current, name);
        if (!nameIndex) continue;
        if (strlen(name) > 0) {
            printf("[%d] [%p] [%p] %s (%u)\n", i, nameIndex, current, name, strlen(name));
        }
    }
    return true;
}


bool traverseUObjectForMembersEtc(HANDLE hProcess, LPVOID baseAddress, uintptr_t _pUObject, size_t size) {
    LPVOID FNamePool = getDynamicMemoryAddress(hProcess, baseAddress, FNamePoolPointerchain, FNamePoolPointerchainSize);
    FNamePoolIntern pool = FNamePoolIntern((uintptr_t)baseAddress + (uintptr_t)FNamePool);

    UObject* pUOBject = (UObject*)malloc(size);

    if (!ReadProcessMemory(hProcess, (LPCVOID)_pUObject, (LPVOID)pUOBject, size, NULL)) {
        printf("[!] Could not read memory (0x%p)\n", _pUObject);
        return false;
    }

    // Assert that UObject points to a valid UObject
    char UObjectName[NAME_LENGTH] = { 0 };
    if (!pool.getName(hProcess, pUOBject->name, UObjectName)) {
        printf("[!] Could not get name for uobject (0x%p)!\n", _pUObject);
        return false;
    }


    if (IsBadReadPtr(hProcess, (LPVOID)pUOBject->pClassPrivate) || IsBadReadPtr(hProcess, (LPVOID)pUOBject->pClassOuter)) {
        printf("pClassPrivate (%p) or pClassOuter (%p) is not a valid pointer ....\n", pUOBject->pClassPrivate, pUOBject->pClassOuter);
        return false;
    }

    printf("[%s]\nFlags: %u\nClassPrivate: %p\nClassOuter: %p\n", UObjectName, pUOBject->flags, pUOBject->pClassPrivate, pUOBject->pClassOuter);
    

    for (int i = sizeof(UObject); i < size; i = i+8) {
        
        uintptr_t ptr = *reinterpret_cast<uintptr_t*>((uintptr_t)(char*)pUOBject + i);
        
        if (IsBadReadPtr(hProcess, (LPVOID)ptr)) continue;
        

        if (ptr > 0x7FF000000000) {
            // Function
        }
        else {


            UObject* pUOBject = (UObject*)malloc(size);

            if (!ReadProcessMemory(hProcess, (LPCVOID)ptr, (LPVOID)pUOBject, size, NULL)) {
                continue;
            }

            if (IsBadReadPtr(hProcess, (LPVOID)pUOBject->pClassPrivate) || IsBadReadPtr(hProcess, (LPVOID)pUOBject->pClassOuter)) {
                continue;
            }


            // Object/Class
            char UObjectName[NAME_LENGTH] = { 0 };
            if (!pool.getName(hProcess, pUOBject->name, UObjectName)) {
                printf("[!] Could not get name for uobject!\n");
                continue;
            }
            printf("-[%p] %s (0x%x)\n", ptr, UObjectName, i);


            uintptr_t child = { 0 };

            if (!ReadProcessMemory(hProcess, (LPCVOID)((char*)_pUObject + i), &child, sizeof(uintptr_t), NULL)) {
                continue;
            }

            //traverseUObjectForMembersEtc(hProcess, baseAddress, child, 0x10);
            free(pUOBject);
        }
        



    }



    free(pUOBject);
}


bool printAllGObjects(HANDLE hProcess, LPVOID baseAddress) {
    //LPVOID addr = getDynamicMemoryAddress(hProcess, baseAddressDataCollector, GObjectsPointerchain, GObjectsPointerchainSize);

    const size_t arraySize = 800000;
    uintptr_t pGObjectsController = 0x00007FF635937A08;

    printf("addr: %p\n", (char*)pGObjectsController + 0x350);
    uintptr_t* GObjectsArray = (uintptr_t*)calloc(arraySize, sizeof(uintptr_t));
    if (!ReadProcessMemory(hProcess, (char*)pGObjectsController + 0x350, GObjectsArray, arraySize * sizeof(uintptr_t), NULL)) {
        printf("[!] Could not read memory! Error %u\n", GetLastError());
        return false;
    }

    return printFNameForUObjects(hProcess, baseAddress, GObjectsArray, arraySize);
}




BOOL printEntitiesNames(HANDLE hProcess, LPVOID baseAddress) {
    EntityWrapper** pEntityList = reinterpret_cast<EntityWrapper**>(getDynamicMemoryAddress(hProcess, baseAddress, ingameEntityListPointerchain, ingameEntityListPointerchainSize));

    UINT8 uAmountEntities = 100;
    int count = 0;
    for (UINT8 i = 0; i < uAmountEntities; i++) {


        // This reads the value that should point to the entity list (Array of pointers)
        EntityWrapper** ppEntityWrapper = nullptr;
        ReadProcessMemory(hProcess, pEntityList, &ppEntityWrapper, sizeof(EntityWrapper**), NULL);

        // Iterate through the list
        LPVOID pCurrentEntity = ppEntityWrapper + i; // + i as ppEntityWrapper is an array of pointers. So its + i(e.g 1) * 8
        //    printf("EntityWrapper* at %p\n", pCurrentEntity);

            // Read the value at the index of the list (Array of pointers)
        EntityWrapper* pEntityWrapper = { 0 };
        ReadProcessMemory(hProcess, pCurrentEntity, &pEntityWrapper, sizeof(EntityWrapper*), NULL);

        // Grab the EntityWrapper
        EntityWrapper entityWrapper = { 0 };
        ReadProcessMemory(hProcess, pEntityWrapper, &entityWrapper, sizeof(EntityWrapper), NULL);
        //    printf("Entity at: %p\n", entityWrapper.pEntity);

            // Grab the entity
        Entity entity = Entity();
        ReadProcessMemory(hProcess, entityWrapper.pEntity, &entity, sizeof(Entity), NULL);


        if (entity.fHealth <= 1.f) {
            //     printf("Probably found the end of the entity list ...\n");
            continue;
        }
        printf("pUObject: %p\n", pEntityWrapper);


        char name[NAME_LENGTH] = { 0 };

        getFNameForUObject(hProcess, baseAddress, (uintptr_t)entityWrapper.pEntity, name);
        printf("[%s] Entity health: %f\n",name, entity.fHealth);


        count++;
    }

    printf("Found %d entities with health\n", count);
    return true;
}

*/