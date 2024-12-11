// Aliens_Fireteam_Elite_external_hack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include "Memory.h"
#include <stdio.h>
#include "game_pointers.h"

#define PROCESS_NAME L"Endeavor-Win64-Shipping.exe"

BOOL setIngameConsumablesTo999(HANDLE hProcess, LPVOID baseAddress);

BOOL removeWeaponSpray(HANDLE hProcess, LPVOID baseAddress) {

    uintptr_t onAimAddress = (uintptr_t)baseAddress + weaponSpreadPatchOnAim.address;

    if (!patchMemory(hProcess, (LPVOID)onAimAddress, weaponSpreadPatchOnAim)) {
        std::cout << "Could not patch memory ..." << std::endl;
        return 1;
    }
    else {
        std::cout << "Patched spread reset on aim" << std::endl;
    }

    uintptr_t onShootAddress = (uintptr_t)baseAddress + weaponSpreadPatchOnShoot.address;

    if (!patchMemory(hProcess, (LPVOID)onShootAddress, weaponSpreadPatchOnShoot)) {
        std::cout << "Could not patch memory ..." << std::endl;
        return 1;
    }
    else {
        std::cout << "Patched spread reset on shoot" << std::endl;
    }


    char buffer[] = { 0x0, 0x0, 0x0, 0x0 }; // 4 Byte
    if (!writeMemory(hProcess, baseAddress, rifleSpreadPointerchain, rifleSpreadPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    else {
        std::cout << "Set spread to 0" << std::endl;
    }

    if (!writeMemory(hProcess, baseAddress, SMGSpreadPointerchain, SMGMagazinePointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    else {
        std::cout << "Set spread to 0" << std::endl;
    }

    return 0;
}


struct threadParam {
    HANDLE hProcess;
    LPVOID baseAddress;
};

void startInfiniteStaminaThread(LPVOID lpParam) {
    threadParam* params = reinterpret_cast<threadParam*>(lpParam);
    std::cout << "Hello from thread: " << GetCurrentThreadId() << std::endl;
    std::cout << "Starting infinite loop" << std::endl;
    while (true) {
        std::cout << "Setting current stamina float to 100f" << std::endl;
        char buffer[] = { 0x0, 0x0, 0xc8, 0x42 }; // float 100.0f little-endian
        if (!writeMemory(params->hProcess, params->baseAddress, staminaPointerchain, staminaPointerchainSize, buffer, sizeof(buffer))) {
            std::cout << "Could not patch memory ...";
            return;
        }
        Sleep(2000);
    }
}


BOOL setIngameTopAndRightConsumableTo99(HANDLE hProcess, LPVOID baseAddress) {
    
    if (setIngameConsumablesTo999(hProcess, baseAddress) > 0) {
        return 1;
    }
    
    char buffer[] = { 0x63, 0x0, 0x0, 0x0 };

    if (!writeMemory(hProcess, baseAddress, ingameConsumableTopAmountPointerchain, ingameConsumableTopAmountPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return 1;
    }

    if (!writeMemory(hProcess, baseAddress, ingameConsumableRightAmountPointerchain, ingameConsumableRightAmountPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not write memory..." << std::endl;
        std::cout << GetLastError() << std::endl;
        return 1;
    }

    return 0;
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
    char buffer[] = { 0xe7, 0x03 }; // 999 little-endian

    for (int i = 0; i < sizeof(consumablePointerMaps)/sizeof(LPVOID*) - 1; i++) {
        if (!writeMemory(hProcess, baseAddress, consumablePointerMaps[i], consumablePointerchainSize, buffer, sizeof(buffer))) {
            std::cout << "Could not patch memory ...";
            return 1;
        }
    }

    return 0;
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
            return 1;
        }
    }

    return 0;
}


// This should not be used as it will NOP all important events and kindof crash the game lol
BOOL removeStaminaStarvation(HANDLE hProcess, LPVOID baseAddress) {

    uintptr_t address = (uintptr_t)baseAddress + staminaReductionDamping.address;

    if (!patchMemory(hProcess, (LPVOID)address, staminaReductionDamping)) {
        std::cout << "Could not patch memory ..." << std::endl;
        return 1;
    }
    else {
        std::cout << "Patched stamina starvation" << std::endl;
    }

    return 0;
}


BOOL setSMGMagazine(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting current magazine ammo to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 };
    if (!writeMemory(hProcess, baseAddress, SMGMagazinePointerchain, SMGMagazinePointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    return 0;
}

BOOL setSMGTotal(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting total ammo for current weapon to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 };
    if (!writeMemory(hProcess, baseAddress, SMGTotalAmmoPointerchain, SMGTotalAmmoPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    return 0;
}



BOOL setRifleMagazine(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting current magazine ammo to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 };
    if (!writeMemory(hProcess, baseAddress, RifleMagazinePointerchain, RifleMagazinePointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    return 0;
}

BOOL setRifleTotal(HANDLE hProcess, LPVOID baseAddress) {
    std::cout << "Setting total ammo for current weapon to 9999 (0x270F)" << std::endl;
    char buffer[] = { 0x0F, 0x27 };
    if (!writeMemory(hProcess, baseAddress, RifleTotalAmmoPointerchain, RifleTotalAmmoPointerchainSize, buffer, sizeof(buffer))) {
        std::cout << "Could not patch memory ...";
        return 1;
    }
    return 0;
}


int main(int argc, char** argv)
{

    DWORD pid = GetAlienFireteamElitePid();

    if (!pid) {
        std::cout << "Process not found ..." << std::endl;
        return 1;
    }
    else {
        std::cout << "Found process. ID: " << pid << std::endl;
    }

    std::cout << "Starting hack ...." << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (!hProcess) {
        std::cout << "Could not get process ..." << std::endl;
        return 1;
    }

    BOOL stop = false;
    uintptr_t baseAddress = GetModuleBaseAddress(pid, PROCESS_NAME);


    threadParam params = {
        hProcess,
        (LPVOID)baseAddress
    };

    while (!stop) {

        char buffer[5]{ 0 };

        printf("Options:\n");
        printf("[In an active game]\n");
        printf("1: Set current weapon ammo to 9999\n");
        printf("2: Toggle weapon spread\n");
        printf("3: Infinite stamina\n");
        printf("5: Set top and right consumable amount to 99\n\n");
        printf("[In the hub-world]\n");
        printf("4: Set all consumables to 999\n");
        printf("0: Exit\n\n\n");
        scanf_s("%s",&buffer,sizeof(buffer));
        fflush(stdin);

        if (strncmp(buffer, "1",1) == 0) {
            if (setSMGMagazine(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            if (setSMGTotal(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            if (setRifleMagazine(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            if (setRifleTotal(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << GetLastError() << std::endl;
                return 1;
            }
        }
        else if (strncmp(buffer, "2", 1) == 0) {
            if (removeWeaponSpray(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << "Could not remove weapon spread" << std::endl;
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            else {
                std::cout << "Removed weapon spread :)" << std::endl;
            }
        }
        else if (strncmp(buffer, "3", 1) == 0) {
            HANDLE hThread = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)startInfiniteStaminaThread, (LPVOID)&params, 0, NULL);
            if (!hThread) {
                std::cout << "Could not start new thread ..." << std::endl;
            }
            else {
                std::cout << "Started new thread. Thread ID: " << GetThreadId(hThread) << std::endl;
            }
        }
        else if (strncmp(buffer, "4", 1) == 0) {
            if (setConsumablesTo999(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << "Could not set consumables" << std::endl;
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            else {
                std::cout << "Set all consumables to 999 :)" << std::endl;
            }
        }
        else if (strncmp(buffer, "5", 1) == 0) {
            if (setIngameTopAndRightConsumableTo99(hProcess, (LPVOID)baseAddress) > 0) {
                std::cout << "Could not set consumable" << std::endl;
                std::cout << GetLastError() << std::endl;
                return 1;
            }
            else {
                std::cout << "Set consumable to 99 :)" << std::endl;
            }
        }

        else if (strncmp(buffer, "0", 1) == 0) {
            break;
        }
        else {
            printf("Unknown...");
        }
        printf("\n\n");

    }

    std::cout << "Exiting hack :)" << std::endl;
}
