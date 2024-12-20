#include "Memory.h"
#include <iostream>
#include "HackLogic.hpp"
#include "game_pointers.h"


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
