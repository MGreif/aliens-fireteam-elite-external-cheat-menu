// Aliens_Fireteam_Elite_external_hack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

#include <TlHelp32.h>
#include "UE_SDK.hpp"

#include "Memory.h"
#include <stdio.h>
#include "LoopThread.hpp"
#include "HackLogic.hpp"

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
bool unreal(HANDLE hProcess, LPVOID baseAddress, Mem *mem);


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

int main(int argc, char** argv)
{
    UE_SDK::init();
    if (argc < 2) {
        printf("usage: hack.exe {sdk,hack}");
        return false;
    }

    if (strncmp(argv[1], "hack", 4) == 0) {
        g_iMode = 2;
    }
    else if (strncmp(argv[1], "sdk", 3) == 0) {
        g_iMode = 1;
    }
    else {
        printf("usage: hack.exe {sdk,hack}");
        return false;
    }

    DWORD pid = GetAlienFireteamElitePid();

    if (!pid) {
        std::cout << "Process not found ..." << std::endl;
        return WIN_ERROR;
    }
    else {
        std::cout << "Found process. ID: " << pid << std::endl;
    }

    std::cout << "Starting hack ...." << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (!hProcess) {
        std::cout << "Could not get process ..." << std::endl;
        return WIN_ERROR;
    }

    Mem mem = Mem(hProcess, PROCESS_NAME);

    mem.moduleBaseAddress = Mem::GetModuleBaseAddress(pid, PROCESS_NAME);
    uintptr_t baseAddress = mem.moduleBaseAddress;

    wprintf(L"Found base address of the %s module: %p\n", PROCESS_NAME, baseAddress);

    if (g_iMode == 1) {
        unreal(hProcess, (LPVOID)baseAddress, &mem);
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


bool unreal(HANDLE hProcess, LPVOID baseAddress, Mem *mem) {
        
    uintptr_t pGObjectsArray = 0x000000007FF7D5357A08;
    uintptr_t pFnamePool = (uintptr_t)Mem::getDynamicMemoryAddress(hProcess, baseAddress, FNamePoolPointerchain, FNamePoolPointerchainSize);

    printf("BaseAddress: %p pFnamePool %p pGObjectsArray %p\n", baseAddress, pFnamePool, pGObjectsArray);


    UE_SDK::Remote_SDK sdk = UE_SDK::Remote_SDK(pGObjectsArray, pFnamePool);
    sdk.initMem(mem);


    if (!UE_SDK::traverseUObjectForMembersEtc( (uintptr_t)0x02085F3AB800, 0x1000,0, 1)) {
        return false;
    }

    return true;

    if (!UE_SDK::printAllGObjects()) {
        return false;
    }
    return true;

    return true;


    /*
    if (!printAllGObjects(hProcess, baseAddress)) {
        return false;
    }

    return true;

    if (printAllFNames(hProcess, baseAddress)) {
        printf("[!]Failed printing all names!\n");
        return false;
    }
    return true;
    */
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
