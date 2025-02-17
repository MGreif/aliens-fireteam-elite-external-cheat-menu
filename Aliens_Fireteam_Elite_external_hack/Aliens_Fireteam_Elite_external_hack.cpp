// Aliens_Fireteam_Elite_external_hack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

#include <TlHelp32.h>
#include "UE_SDK.hpp"

#include "Memory.h"
#include <stdio.h>
#include <sstream>
#include "hack\LoopThread.hpp"
#include "hack\HackLogic.hpp"

#define PROCESS_NAME L"Endeavor-Win64-Shipping.exe"
#define WIN_ERROR 1
#define WIN_SUCCESS 0
extern MemoryPatchLoopUnitOfWork* unitsOfWork[];
extern UINT8 unitsOfWorkSize;
extern MemoryPatchLoopUnitOfWork infiniteStaminaUnitOfWork;
extern MemoryPatchLoopUnitOfWork noRecoilUnitOfWork;
extern MemoryPatchLoopUnitOfWork godmodeUnitOfWork;
extern MemoryPatchLoopUnitOfWork instakillUnitOfWork;

bool hack(HANDLE hProcess, uintptr_t baseAddress);
bool sdk(HANDLE hProcess, LPVOID baseAddress, Mem *mem, int argc, char** argv);


int g_iMode = 0; // 1 for hack, 2 for sdk
extern UE_SDK::Remote_SDK* pSdk ;


void printUsage(bool bClearScreenBefore) {
    if (bClearScreenBefore) {
        system("cls");
    }
    printf("Options:\n");
    printf("[In an active game]\n");
    printf("1: Set current weapon ammo to 9999\n");
    printf("2: Remove weapon spread\n");
    printf("3: Toggle Infinite stamina %s\n", infiniteStaminaUnitOfWork.active ? "\x1B[31m[active]\x1B[97m" : "");
    printf("5: Set top and right consumable amount to 99\n");
    printf("6: Set max walk speed to 5000\n");
    printf("7: Set sprint speed to 5000\n");
    printf("8: Toggle weapon no-recoil %s\n", noRecoilUnitOfWork.active ? "\x1B[31m[active]\x1B[97m" : "");
    printf("9: Toggle godmode %s\n\n", godmodeUnitOfWork.active ? "\x1B[31m[active]\x1B[97m" : "");
    printf("[In the hub-world]\n");
    printf("4: Set all consumables to 999\n");
    printf("0: Exit\n\n\n");
}

void printSDKUsage() {
    error("usage: hack.exe {sdk,hack}\n");
    printf("\thack.exe sdk print-names                             # Prints all GObjects names\n");
    printf("\thack.exe sdk object-info <address>  <max-levels>     # Prints info about a specified UOBject\n");
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        error("usage: hack.exe {sdk,hack}");
        return false;
    }

    if (strncmp(argv[1], "hack", 4) == 0) {
        g_iMode = 2;
    }
    else if (strncmp(argv[1], "sdk", 3) == 0) {
        g_iMode = 1;
    }
    else {
        error("usage: hack.exe {sdk,hack}");
        return false;
    }

    DWORD pid = GetAlienFireteamElitePid();

    if (!pid) {
        error("Process not found ...\n");
        return WIN_ERROR;
    }
    else {
        success("Found process.ID: %p\n", pid);
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (!hProcess) {
        error("Could not get process ...\n");

        return WIN_ERROR;
    }

    Mem mem = Mem(hProcess, PROCESS_NAME);

    mem.moduleBaseAddress = Mem::GetModuleBaseAddress(pid, PROCESS_NAME);
    uintptr_t baseAddress = mem.moduleBaseAddress;

    wprintf(L"Found base address of the %s module: %p\n", PROCESS_NAME, baseAddress);

    if (g_iMode == 1) {
        if (argc <= 2) {
            printSDKUsage();
            return false;
        }
        sdk(hProcess, (LPVOID)baseAddress, &mem, argc, argv);
    }
    else if (g_iMode == 2) {
        return hack(hProcess, baseAddress);
    }

    return true;
}

LPVOID FNamePoolPointerchain[]{
    (LPVOID)0x518EE98,
    (LPVOID)0x0
};

UINT8 FNamePoolPointerchainSize = 2;


bool sdk(HANDLE hProcess, LPVOID baseAddress, Mem *mem, int argc, char** argv) {

    uintptr_t pFnamePool = (uintptr_t)mem->getDynamicMemoryAddress(FNamePoolPointerchain, FNamePoolPointerchainSize);
    char GObjectsPattern[] = { 0x05, 0x00, 0x05, 0x07, 0xC3, 0x1F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x51, 0x00, 0x00, 0xFD, 0x51, 0x00, 0x00 };

    uintptr_t pGObjectsArray = mem->findPattern(GObjectsPattern, sizeof(GObjectsPattern));

    if (pGObjectsArray > 0) {
        pGObjectsArray += 0x10; // Add AOB scan offset for correct address
    }
    else {
        error("[findGOBjects] Could not find GObjects\n");
        return false;
    }

    debug("BaseAddress: %p pFnamePool %p pGObjectsArray %p\n", baseAddress, pFnamePool, pGObjectsArray);

    UE_SDK::Remote_SDK sdk = UE_SDK::Remote_SDK(pGObjectsArray, pFnamePool);
    sdk.initMem(mem);

    sdk.buildUObjectArray(500000);
    info("Initialized GOBjects with %u objects\n", sdk.pUObjectsSize);

    if (strncmp(argv[2], "print-names", 12) == 0) {
        info("Printing UOBject names from GOBjects\n");
        UE_SDK::printFNameForUObjects((uintptr_t*)sdk.pUObjects, sdk.pUObjectsSize);
    }
    else if (strncmp(argv[2], "object-info", 12) == 0) {
        char* pUObjectArg = argv[3];
        UINT8 maxLevel = 1;
        if (argc < 3 || strlen(pUObjectArg) == 0) {
            printSDKUsage();
            return WIN_ERROR;
        }else if (argc > 4) {
            if (strlen(argv[4]) == 0) {
                printSDKUsage();
                return WIN_ERROR;
            }
            maxLevel = atoi(argv[4]);
            if (maxLevel == 0) {
                printSDKUsage();
                return WIN_ERROR;
            }
        }

        uintptr_t pUObject = (uintptr_t)std::stoull(pUObjectArg, nullptr, 16);
        info("Object information for: 0x%p. Max Level: %u\n", pUObject, maxLevel);
        UE_SDK::traverseUObjectForMembersEtc(pUObject, 0x4000, 0, maxLevel);
    }
    else {
        printSDKUsage();
        return false;
    }

    return true;
}


bool hack(HANDLE hProcess, uintptr_t baseAddress) {
    BOOL stop = false;

    MemoryPatchLoopParam params = {
        hProcess,
        (LPVOID)baseAddress,
        unitsOfWork,
        unitsOfWorkSize
    };

    HANDLE hThread = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)startMemoryPatchLoop, (LPVOID)&params, 0, NULL);
    if (!hThread) {
        std::cout << "Could not start new thread ..." << std::endl;
    }
    else {
        std::cout << "Started new thread. Thread ID: " << GetThreadId(hThread) << std::endl;
    }

    bool bKeyCurrentlyPressed = false;


    printUsage(true);

    while (!stop) {

        char buffer[5]{ 0 };


        fflush(stdin);
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!setPrimaryMagazine(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setPrimaryTotal(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setSecondaryMagazine(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setSecondaryTotal(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }

        }
        else if (GetAsyncKeyState(VK_NUMPAD2) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!removeWeaponSpray(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not remove weapon spread" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Removed weapon spread :)" << std::endl;
            }
        }
        else if (GetAsyncKeyState(VK_NUMPAD3) & 0x01 && !bKeyCurrentlyPressed) {
            infiniteStaminaUnitOfWork.active = !infiniteStaminaUnitOfWork.active;
            printUsage(true);
        }
        else if (GetAsyncKeyState(VK_NUMPAD4) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!setConsumablesTo999(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set consumables" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set all consumables to 999 :)" << std::endl;
            }
        }
        else if (GetAsyncKeyState(VK_NUMPAD5) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!setIngameTopAndRightConsumableTo99(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set consumable" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set consumable to 99 :)" << std::endl;
            }
        }
        else if (GetAsyncKeyState(VK_NUMPAD6) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!setWalkSpeedTo5000(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set walk speed" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set walk speed to 5000 :). This might get reset on walk speed changes in game" << std::endl;
            }
        }
        else if (GetAsyncKeyState(VK_NUMPAD7) & 0x01 && !bKeyCurrentlyPressed) {
            printUsage(true);
            if (!setSprintSpeedTo5000(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set sprint speed" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set sprint speed to 5000 :). This might get reset on walk speed changes in game" << std::endl;
            }
        }
        else if (GetAsyncKeyState(VK_NUMPAD8) & 0x01 && !bKeyCurrentlyPressed) {
            noRecoilUnitOfWork.active = !noRecoilUnitOfWork.active;
            printUsage(true);
        }
        else if (GetAsyncKeyState(VK_NUMPAD9) & 0x01 && !bKeyCurrentlyPressed) {
            godmodeUnitOfWork.active = !godmodeUnitOfWork.active;
            printUsage(true);
        }
        else if (GetAsyncKeyState(VK_ADD) & 0x01 && !bKeyCurrentlyPressed) {
            instakillUnitOfWork.active = !instakillUnitOfWork.active;
            printUsage(true);
        }

        else if (GetAsyncKeyState(VK_NUMPAD0) & 0x01 && !bKeyCurrentlyPressed) {
            break;
        }
        else {
            if (bKeyCurrentlyPressed == true) bKeyCurrentlyPressed = false;
        }
        Sleep(100);
    }

    std::cout << "Exiting hack :)" << std::endl;
}
